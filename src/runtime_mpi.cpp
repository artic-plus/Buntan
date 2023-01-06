#include <starpu.h>
#include <starpu_mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>

#include "frontend.hpp"
#include "nodetypes.hpp"
#include "backend.hpp"


TFHEpp::EvalKey ek;

int main(int argc, char** argv){
    
    int my_rank, size;
    int ret = starpu_mpi_init_conf(&argc, &argv, 1, MPI_COMM_WORLD, NULL);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_mpi_init_conf");
	starpu_mpi_comm_rank(MPI_COMM_WORLD, &my_rank);
	starpu_mpi_comm_size(MPI_COMM_WORLD, &size);
    num_workers = starpu_cpu_worker_get_count();
	if (num_workers == 0)
	{
		std::cerr << "We need at least 1 CPU worker." << std::endl;
		starpu_mpi_shutdown();
		if (my_rank == 0) return 77; else return 0;
	}
    MPI_Datatype typeKey, typet_val;
    MPI_Type_contiguous(sizeof(TFHEpp::EvalKey), MPI_CHAR, &typeKey);
    MPI_Type_commit(&typeKey);
    MPI_Type_contiguous(sizeof(t_val), MPI_CHAR, &typet_val);
    MPI_Type_commit(&typet_val);
#ifndef plainmode
    MPI_Request ReqKey;
#endif

    types_init();

    std::map<std::string, std::pair<int, wire**>> *inputs;
    std::map<std::string, std::pair<int, wire**>> *outputs;
    std::map<std::string, std::pair<node*, t_val*>> *FFs;
    wire* ImmTrue; 
    wire* ImmFalse;
    int numwires, numnodes, numretvals;
    std::vector<starpu_data_handle_t>* retval_handles;
    starpu_data_handle_t retvals_v_handle;
#ifndef plain_mode
    if(my_rank == 0){
        const std::string path = "./cloud.key";
        std::ifstream ifs("./cloud.key", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        (&ek)->serialize(ar);
    }
    MPI_Ibcast(&ek, 1, typeKey, 0, MPI_COMM_WORLD, &ReqKey);
#endif
    if(my_rank == 0){
        inputs = new std::map<std::string, std::pair<int, wire**>>;
        outputs = new std::map<std::string, std::pair<int, wire**>>; 
        FFs = new std::map<std::string, std::pair<node*, t_val*>>;
        ImmTrue = new wire{nullptr, new std::queue<node*>, false, true, false}; 
        ImmFalse = new wire{nullptr, new std::queue<node*>, false, true, false};
        numwires = yosys_json_parser(std::string(argv[1]), inputs, outputs, FFs, ImmTrue, ImmFalse);
        if(numwires < 0){
            std::cerr << "frontend failed!" << std::endl;
            return 1;
        }
        init_FFs(FFs);
#ifndef plain_mode
        MPI_Status status;
        MPI_Wait(&ReqKey, &status);
#endif
        MPI_Bcast(&numwires, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    } else {
#ifndef plain_mode
        MPI_Status status;
        MPI_Wait(&ReqKey, &status);
#endif
        MPI_Bcast(&numwires, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    std::vector<std::pair<node, starpu_data_handle_t*>>* tasks;
    starpu_data_handle_t wires_handle[numwires] = {0};
    if(my_rank == 0){
#ifdef plain_mode
        std::vector<bool> args_in{};
#else
        std::vector<TFHEpp::TLWE<TFHEpp::lvl1param>> args_in{};
#endif
        {
            std::ifstream ifs("./cloud.data", std::ios::binary);
            cereal::PortableBinaryInputArchive ar(ifs);
            ar(args_in);
        }
        *retval_handles = {};
        *tasks = reg_handles_mpi(*inputs, args_in, *outputs, retval_handles, *FFs, ImmTrue, ImmFalse);
        numretvals = retval_handles->size();
    } else {
        int i[2];
        while(1){
            MPI_Status status;
            MPI_Recv(i, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            if(i[0] < 0) break;
            if(i[1] == 0) {
                auto val = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
                *val = false;
#else
                TFHEpp::HomCONSTANTZERO(*val);
#endif
            starpu_variable_data_register(&(wires_handle[i[0]]), STARPU_MAIN_RAM, (uintptr_t)val, sizeof(t_val));
            } else if(i[1] == 1) {
                auto val = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
                *val = true;
#else
                TFHEpp::HomCONSTANTZERO(*val);
#endif
            starpu_variable_data_register(&(wires_handle[i[0]]), STARPU_MAIN_RAM, (uintptr_t)val, sizeof(t_val));
            } else {
                starpu_variable_data_register(&(wires_handle[i[0]]), -1, NULL, sizeof(t_val));
            }
        }
    }
    MPI_Bcast(&numretvals, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for(auto task : *tasks){
        reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(task.first.type->task_insert_mpi)(task.first, task.second);
    }
    if(my_rank == 0){
        std::vector<t_val> retvals(numretvals);
        starpu_vector_data_register(&retvals_v_handle, STARPU_MAIN_RAM, (uintptr_t)&(retvals[0]), retvals.size(), sizeof(t_val));
        for(int i = 0; i < numretvals; i++){
            //TODO : implement cl to copy retvals to vector and register here
        }
        starpu_mpi_wait_for_all(MPI_COMM_WORLD);
        {
            std::ofstream ofs{"result.data", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ar(retvals);
        };
    } else {
        for(int i = 0; i < numretvals; i++){
            //TODO : implement cl to copy retvals to vector and register here
        }
        starpu_mpi_wait_for_all(MPI_COMM_WORLD);
    }
    return 0;
}
