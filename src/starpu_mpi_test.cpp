#include <stdio.h>
#include <starpu.h>
#include <starpu_mpi.h>

// a codelet that initializes a data handle with a MPI process' world rank

void write_number_cpu(void *buffers[], void *cl_arg)
{
    int world_rank = starpu_mpi_world_rank();
    int *value = (int *) STARPU_VARIABLE_GET_PTR(buffers[0]);

    *value = world_rank;
    printf("Rank %d writes value %d.\n", world_rank, *value);
}

struct starpu_codelet write_number_cl = {
    .cpu_funcs = { write_number_cpu },
    .nbuffers = 1,
    .modes = { STARPU_W }
};

// a codelet that prints the contents of a data handle

void read_number_cpu(void *buffers[], void *cl_arg)
{
    int world_rank = starpu_mpi_world_rank();
    int value = *((int *) STARPU_VARIABLE_GET_PTR(buffers[0]));
    printf("Rank %d reads value %d.\n", world_rank, value);
}

struct starpu_codelet read_number_cl = {
    .cpu_funcs = { read_number_cpu },
    .nbuffers = 1,
    .modes = { STARPU_R }
};

int main(int argc, char **argv) {

    // initialize MPI
    int thread_support;
    MPI_Init_thread(
        &argc, (char ***)&argv, MPI_THREAD_MULTIPLE, &thread_support);

    // initialize StarPU
    if (starpu_init(NULL) != 0)
        printf("Failed to initialize Starpu.\n");

    // initialize StarPU-MPI
    if (starpu_mpi_init(&argc, &argv, 0) != 0)
        printf("Failed to initialize Starpu-MPI.\n");

    // query world communicator's size
    int world_size = starpu_mpi_world_size();

    // initialize all data handles
    starpu_data_handle_t handles[world_size];
    for (int i = 0; i < world_size; i++) {

        // register a data handle that is going to be initialized later
        starpu_variable_data_register(
            &handles[i], -1, (uintptr_t) NULL, sizeof(int));

        // register data handle's owner and tag
        starpu_mpi_data_register(handles[i], i, i);
    }

    // insert tasks that initialize the data handles
    for (int i = 0; i < world_size; i++)
        starpu_mpi_task_insert(
            MPI_COMM_WORLD, &write_number_cl,
            STARPU_EXECUTE_ON_DATA, handles[i], // data handles owner executes
            STARPU_W, handles[i], 0);

    // insert tasks that print the data handles
    for (int i = 0; i < world_size; i++)
        starpu_mpi_task_insert(
            MPI_COMM_WORLD, &read_number_cl,
            STARPU_EXECUTE_ON_NODE, i,          // rank i executes
            STARPU_R, handles[(i+1)%world_size], 0);

    // unregister all data handles
    for (int i = 0; i < world_size; i++)
        starpu_data_unregister(handles[i]);

    // de-initialize everything
    starpu_mpi_shutdown();
    starpu_shutdown();
    MPI_Finalize();

    return 0;
}