#include <starpu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <random>
#include <chrono>
#include <vector>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>

#include "frontend.hpp"
#include "nodetypes.hpp"
#include "backend.hpp"

int world_size;
TFHEpp::EvalKey ek;
std::vector<t_val> args_in;

void load_data(void *buffers[], void *cl_arg){
    int numargs, n;
    starpu_codelet_unpack_args(cl_arg, &numargs, &n);
    for(int i = 0; i < numargs; i++){
        *(t_val*)STARPU_VARIABLE_GET_PTR(buffers[i]) = args_in[n * numargs + i];
    }
}

void save_result(void *buffers[], void *cl_arg){
    std::vector<t_val> retvals;
    int numretvals, n;
    starpu_codelet_unpack_args(cl_arg, &numretvals, &n);
    for(int i = 0; i < numretvals * n; i++){
        retvals.push_back(*(t_val*)STARPU_VARIABLE_GET_PTR(buffers[i]));
    }
    {
        std::ofstream ofs{"result.data", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        ar(retvals);
    }
}



int main(int argc, char** argv){
    int n = 1;
    std::string filepath = "./circuit.json";
    for(int i = 0; i < argc - 1; i++){
        if(!strcmp(argv[i], "--circuit"))
            filepath = std::string(argv[i+1]);
        if(!strcmp(argv[i], "--repetition"))
            n = atoi(argv[i+1]);
    }
    // initialize StarPU
    if (starpu_init(NULL) != 0)
        printf("Failed to initialize Starpu.\n");


    

#ifdef perf_measure
    std::chrono::system_clock::time_point start, init, shutdown;
    //start = MPI_Wtime();
    start = std::chrono::system_clock::now();
#endif    


    std::map<std::string, std::pair<int, wire**>> *inputs;
    std::map<std::string, std::pair<int, wire**>> *outputs;
    std::map<std::string, std::pair<node*, t_val*>> *FFs;
    wire* ImmTrue; 
    wire* ImmFalse;
    int numwires[4];
    int numnodes;
    starpu_data_handle_t retvals_v_handle;
    t_val** retval_ptrs;
    int* arg_handle_id; 
    int* retval_handle_id;
    
#ifdef plain_mode
    types_init(1);
#else
    types_init(0);
    {
        const std::string path = "./cloud.key";
        std::ifstream ifs("./cloud.key", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        (&ek)->serialize(ar);
    }
#endif
    inputs = new std::map<std::string, std::pair<int, wire**>>;
    outputs = new std::map<std::string, std::pair<int, wire**>>; 
    FFs = new std::map<std::string, std::pair<node*, t_val*>>;
    ImmTrue = new wire{nullptr, new std::queue<node*>, false, -1}; 
    ImmFalse = new wire{nullptr, new std::queue<node*>, false, -1};
    yosys_json_parser(filepath, numwires, inputs, outputs, FFs, ImmTrue, ImmFalse);
    if(numwires[0] < 0){
        std::cerr << "frontend failed!" << std::endl;
        return 1;
    }
    if(FFs->size() > 0)init_FFs(FFs);
    args_in = std::vector<t_val>{};
    {
        std::ifstream ifs("./cloud.data", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        ar(args_in);
    }

    std::vector<int>* tasks;
#ifdef use_simple_FF
    int FF_io[numwires[3] * 2];
#endif
    starpu_data_handle_t wire_handles[numwires[0]] = {0};
    
    int tasksize;
    enum starpu_data_access_mode modes_load[numwires[1]] = {STARPU_W};
    struct starpu_codelet load_cl = {
        .cpu_funcs = {load_data},
        .nbuffers = numwires[1],
        .dyn_modes = modes_load,
    };    
    arg_handle_id = (int*)calloc(numwires[1], sizeof(int));
    retval_handle_id = (int*)calloc(numwires[2], sizeof(int));
    retval_ptrs = (t_val**)calloc(numwires[2], sizeof(t_val*));
    tasks = reg_handles(*inputs, arg_handle_id, *outputs, retval_handle_id,  retval_ptrs, *FFs, wire_handles, numwires[0], ImmTrue, ImmFalse);
    tasksize = tasks->size();
#ifdef use_simple_FF
    int FF_index = 0;
    for(auto FF = FFs->begin(); FF != FFs->end(); FF++){
        FF_io[FF_index] = (int)(intptr_t)FF->second.first->inputs[0].first;
        FF_index++;
        FF_io[FF_index] = (int)(intptr_t)FF->second.first->dff_mem;
        FF_index++;
    }
#endif

    auto arg_descrs = (struct starpu_data_descr*)calloc(numwires[1], sizeof(struct starpu_data_descr));
    for(int i = 0; i < numwires[1]; i++){
        arg_descrs[i].handle = wire_handles[arg_handle_id[i]];
        arg_descrs[i].mode = STARPU_W;
    }
    starpu_data_handle_t retvals[n * numwires[2]];
    for(int i = 0; i < n * numwires[2]; i++){
        t_val* ret = (t_val*)calloc(1, sizeof(t_val));
        starpu_variable_data_register(&retvals[i], STARPU_MAIN_RAM, (uintptr_t)ret, sizeof(t_val));
    }
    auto retval_descrs = (struct starpu_data_descr*)calloc(n * numwires[2], sizeof(struct starpu_data_descr));
    for(int i = 0; i < n * numwires[2]; i++){
        retval_descrs[i].handle = retvals[i];
        retval_descrs[i].mode = STARPU_R;
    }
    enum starpu_data_access_mode modes_save[n * numwires[2]] = {STARPU_R};
    struct starpu_codelet save_cl = {
        .cpu_funcs = {save_result},
        .nbuffers = n * numwires[2],
        .dyn_modes = modes_save,
    };
#ifdef perf_measure
    init = std::chrono::system_clock::now();
#endif
    for(int t = 0; t < n; t++){
        starpu_task_insert(&load_cl,
            STARPU_VALUE, &(numwires[1]), sizeof(int),
            STARPU_VALUE, &t, sizeof(int),
            STARPU_DATA_MODE_ARRAY, arg_descrs, numwires[1],
            0);
        int task_index = 0;
        int task_id = 0;
        while(task_index < tasksize){
            nodetype* type = type_id[(*tasks)[task_index]];
            task_index++;
            auto wire_descrs = (struct starpu_data_descr*)calloc(type->inputs.size() + type->outputs.size(), sizeof(struct starpu_data_descr));
            for(int i = 0; i < type->inputs.size(); i++){
                wire_descrs[i].handle = wire_handles[(*tasks)[task_index]];
                wire_descrs[i].mode = STARPU_R;
                task_index++;
            }
            for(int i = 0; i < type->outputs.size(); i++){
                wire_descrs[type->inputs.size() + i].handle = wire_handles[(*tasks)[task_index]];
                wire_descrs[type->inputs.size() + i].mode = STARPU_RW;
                task_index++;
            }
            starpu_task_insert((starpu_codelet*)type->cl,
                STARPU_DATA_MODE_ARRAY, wire_descrs, type->inputs.size() + type->outputs.size(),
                0);
            task_id++;
        }
#ifdef use_simple_FF
        for(int i = 0; i < numwires[3]; i++){
            starpu_task_insert(&copy_cl,
                STARPU_R, wire_handles[FF_io[2*i]],
                STARPU_W, wire_handles[FF_io[2*i+1]],
                0);
        }
#endif
        for(int i = 0; i < numwires[2]; i++){
            starpu_task_insert(&copy_cl,
                STARPU_R, wire_handles[retval_handle_id[i]],
                STARPU_W, retvals[i + t * numwires[2]],
                0);
        }
    }
        starpu_task_insert(&save_cl,
            STARPU_VALUE, &(numwires[2]), sizeof(int),
            STARPU_VALUE, &n, sizeof(int),
            STARPU_DATA_MODE_ARRAY, retval_descrs, n * numwires[2],
            0);
    
    for(int i = 0; i < numwires[0]; i++){
        starpu_data_unregister(wire_handles[i]);
    }
    for(int i = 0; i < n * numwires[2]; i++){
        starpu_data_unregister(retvals[i]);
    }
    starpu_shutdown();
    #ifdef perf_measure
    shutdown = std::chrono::system_clock::now();
    double time = static_cast<double>(
        std::chrono::duration_cast<std::chrono::microseconds>(init - start)
            .count() /
        1000.0);
    std::cout << "init time : " << time << "[ms]" << std::endl;
    time = static_cast<double>(
        std::chrono::duration_cast<std::chrono::microseconds>(shutdown - start)
            .count() /
        1000.0);
    std::cout << "total time : " << time << "[ms]" << std::endl;
#endif
    return 0;
}
