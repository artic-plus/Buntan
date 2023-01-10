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

void copy_plain(void *buffers[], void *cl_arg){
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) = *(bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
}

void copy_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::HomCOPY(*Y, *A);
}

struct starpu_codelet copy_cl = {
#ifdef plain_mode
	.cpu_funcs = {copy_plain},
#else
    .cpu_funcs = {copy_cipher},
#endif
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_W}
};


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
    auto tasks = new std::vector<int>;
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
        for(int j = 0; j < num_mems; j++){
            starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)&(FF->second.second[j]), sizeof(t_val));
            starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
            wire_index++;
        }
    }
    t_val* Immt = (t_val*)malloc(sizeof(t_val));
    t_val* Immf = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
    *Immt = true;
    *Immf = false;
#else
    TFHEpp::HomCONSTANTONE(*Immt);
    TFHEpp::HomCONSTANTZERO(*Immf);
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
            starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)arg, sizeof(*Immf));
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
                for(int i = 0; i < nextnode->num_inputs; i++) tasks->push_back((int)(intptr_t)nextnode->inputs[i].first);
                for(int i = 0; i < nextnode->num_outputs; i++){
                    t_val* out_b = (t_val*)calloc(1, sizeof(t_val));
                    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)out_b , sizeof(t_val));
                    starpu_mpi_data_register(wire_handles[wire_index], wire_index, 0);
                    if(nextnode->outputs[i]->output_id >= 0){
                        retval_handle_id[nextnode->outputs[i]->output_id] = wire_index;
                        retval_ptrs[nextnode->outputs[i]->output_id] = &(out_b[i]);
                    }
                    tasks->push_back(wire_index);
                    wires_queue.push(std::make_pair(nextnode->outputs[i], wire_index));
                    wire_index++;
                }
                nextnode->d_counter = nextnode->num_inputs;
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
        for(int i = 0; i < num_mems; i++) tasks->push_back((int)(intptr_t)ff_node->dff_mem + i);
        for(int i = 0; i < ff_node->num_inputs; i++) tasks->push_back((int)(intptr_t)ff_node->inputs[i].first);
    }
#endif
    return tasks;

}

