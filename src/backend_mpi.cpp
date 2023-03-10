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



/* Returns the MPI node number where data indexes index is */
int gate_distrib(int gate_index)
{
	return gate_index % world_size;
}



//return a vector of inputs for "insert_***_mpi" functions
std::vector<int>* reg_handles_mpi(
    std::map<std::string, std::pair<int, wire**>> inputs, 
    int* arg_handle_id, 
    std::map<std::string, std::pair<int, wire**>> outputs, 
    int* retval_handle_id,
    t_val** retval_ptrs,
    std::map<std::string, std::pair<node*, t_val*>> FFs, 
    starpu_data_handle_t* wire_handles,
    int numwires,
    wire* ImmTrue, wire* ImmFalse
){
    auto tasks = new std::vector<int>{};
    std::map<wire*, int> wires;
    std::queue<std::pair<wire*, int>> wires_queue;
    std::map<wire*, t_val*> wires_ptr;

    int wire_index = 0;

    //register Q-value of FF at first to keep coherency
    if(FFs.size())
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        //std::cout << "FF   :" << FF->first << " " << FF->second->num_outputs << "bit" << std::endl;
        FF->second.first->dff_mem = (uintptr_t)wire_index;
        wires_queue.push(std::make_pair(FF->second.first->outputs[0], wire_index));
        wires_ptr.insert(std::make_pair(FF->second.first->outputs[0], (t_val*)FF->second.second));
        if(FF->second.first->outputs[0]->output_id >= 0){
            retval_handle_id[FF->second.first->outputs[0]->output_id] = wire_index;
            retval_ptrs[FF->second.first->outputs[0]->output_id] = (FF->second.second);
        }
        starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)FF->second.second, sizeof(t_val) * num_mems);
        starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
        wire_index++;
    }
    t_val* Immt = (t_val*)malloc(sizeof(t_val));
    t_val* Immf = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
    *Immt = true;
    *Immf = false;
#else
    c_one(*Immt);
    c_zero(*Immf);
#endif

    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)Immt, sizeof(*Immt));
    starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
    wires_ptr.insert(std::make_pair(ImmTrue, Immt));
    wires.insert(std::make_pair(ImmTrue, wire_index));
    if(ImmTrue->output_id >= 0){
        retval_handle_id[ImmTrue->output_id] = wire_index;
        retval_ptrs[ImmTrue->output_id] = (Immt);
    }
    wire_index++;

    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)Immf, sizeof(*Immf));
    starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
    wires_ptr.insert(std::make_pair(ImmFalse, Immf));
    wires_queue.push(std::make_pair(ImmFalse, wire_index));
    if(ImmFalse->output_id >= 0){
        retval_handle_id[ImmFalse->output_id] = wire_index;
        retval_ptrs[ImmFalse->output_id] = (Immf);
    }
    wire_index++;
    int arg_index = 0;
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        for(int i = 0; i < it->second.first; i++){
            t_val* arg = (t_val*)calloc(1, sizeof(t_val));
            starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)arg, sizeof(t_val));
            starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
            wires_ptr.insert(std::make_pair(it->second.second[i], Immf));
            wires_queue.push(std::make_pair(it->second.second[i], wire_index));
            if(it->second.second[i]->output_id >= 0){
                retval_handle_id[it->second.second[i]->output_id] = wire_index;
                retval_ptrs[it->second.second[i]->output_id] = (arg);
            }
            arg_handle_id[arg_index] = wire_index;
            wire_index++;
            arg_index++;
        }
    }

    
    wire* next;
    int next_handle_id;
    int task_id = 0;
    while(!wires_queue.empty())
    {
        next = wires_queue.front().first;
        next_handle_id = wires_queue.front().second;
        auto newqueue = new std::queue<node*>;
        node* nextnode;
        while (!next->dep->empty())
        {
            nextnode = next->dep->front();
            for(int i = 0; i < nextnode->num_inputs; i++){
                if(nextnode->inputs[i].second == next){
                    nextnode->inputs[i].first = (void*)next_handle_id;
                    nextnode->d_counter--;
                }
            }
            if((nextnode->d_counter == 0) && !(nextnode->type->isFF)){
                tasks->push_back(nextnode->type->id);
                tasks->push_back(gate_distrib(task_id));
                for(int i = 0; i < nextnode->num_inputs; i++) tasks->push_back((int)(intptr_t)nextnode->inputs[i].first);
                for(int i = 0; i < nextnode->num_outputs; i++){
                    t_val* out_b = (t_val*)calloc(1, sizeof(t_val));
                    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)out_b , sizeof(t_val));
                    starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
                    if(nextnode->outputs[i]->output_id >= 0){
                        retval_handle_id[nextnode->outputs[i]->output_id] = wire_index;
                        retval_ptrs[nextnode->outputs[i]->output_id] = out_b;
                    }
                    tasks->push_back(wire_index);
                    wires_queue.push(std::make_pair(nextnode->outputs[i], wire_index));
                    wire_index++;
                }
                nextnode->d_counter = nextnode->num_inputs;
                task_id++;
            } 
            newqueue->push(next->dep->front());
            next->dep->pop();
        }
        delete next->dep;
        next->dep = newqueue;
        wires.insert(wires_queue.front());
        wires_queue.pop();
    }
    while(wire_index < numwires){
        starpu_variable_data_register(&(wire_handles[wire_index]), -1, (uintptr_t)NULL, sizeof(t_val));
        starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
        wire_index++;
    }
#ifndef use_simple_FF
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        node* ff_node = FF->second.first;
        tasks->push_back(ff_node->type->id);
        for(int i = 0; i < ff_node->num_inputs; i++) tasks->push_back((int)(intptr_t)ff_node->inputs[i].first);
        tasks->push_back((int)(intptr_t)ff_node->dff_mem);
    }
#endif
    return tasks;

}

int insert_tasks_mpi(
    std::vector<int>* tasks,
    starpu_data_handle_t* wire_handles
){
    int task_index = 0;
    int task_id = 0;
    while(task_index < tasksize){
        nodetype* type = type_id[(*tasks)[task_index]];
        task_index++;
        int distrib = (*tasks)[task_index];
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
        starpu_mpi_task_insert(MPI_COMM_WORLD, (starpu_codelet*)type->cl,
            STARPU_EXECUTE_ON_NODE, distrib,
            STARPU_DATA_MODE_ARRAY, wire_descrs, type->inputs.size() + type->outputs.size(),
            0);
        task_id++;
    }
}