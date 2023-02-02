#include <starpu.h>
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

#ifdef plain_mode
extern struct starpu_codelet init_cl = init_p_cl;
#else
extern struct starpu_codelet init_cl = init_c_cl;
#endif


#ifdef plain_mode
extern struct starpu_codelet copy_cl = copy_p_cl;
#else
extern struct starpu_codelet copy_cl = copy_c_cl;
#endif


#ifdef use_simple_FF
    int num_mems = 1;
#else
    int num_mems = 3; // for state, fCLK, and fRST
#endif


int init_FFs(std::map<std::string, std::pair<node*, t_val*>>* FFs){
    for(auto FF = FFs->begin(); FF != FFs->end(); FF++){
        FF->second.second = (t_val*)calloc(num_mems, sizeof(t_val));
#ifndef plain_mode
        for(int i = 0; i < num_mems; i++)
            TFHEpp::HomCONSTANTZERO<lvl_param>(FF->second.second[i]);
#endif
    }
    return 0;
}






int deploygates(
    std::map<std::string, std::pair<int, wire**>> inputs,
    t_val* arg_in,
    std::map<std::string, std::pair<int, wire**>> outputs, 
    std::vector<t_val>* retvals, 
    std::map<std::string, std::pair<node*, t_val*>> FFs, 
    wire* ImmTrue, 
    wire* ImmFalse
){
    std::vector<std::pair<wire*, starpu_data_handle_t*>> wires;
    std::map<wire*, t_val*> wires_ptr;

    std::map<std::string, std::pair<int, t_val**>> output_handles; 
    for(auto it = outputs.begin() ; it != outputs.end(); it++){
        t_val** ptrs = (t_val**)calloc(it->second.first, sizeof(t_val*));
        output_handles.insert(std::make_pair(it->first, std::make_pair(it->second.first, ptrs)));
    }

    t_val* Immt = (t_val*)malloc(sizeof(t_val));
    t_val* Immf = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
    *Immt = true;
    *Immf = false;
#else
    TFHEpp::HomCONSTANTONE<lvl_param>(*Immt);
    TFHEpp::HomCONSTANTZERO<lvl_param>(*Immf);
#endif

    auto handle_T = new starpu_data_handle_t;
    auto handle_F = new starpu_data_handle_t;
    starpu_variable_data_register(handle_T, STARPU_MAIN_RAM, (uintptr_t)Immt, sizeof(*Immt));
    starpu_variable_data_register(handle_F, STARPU_MAIN_RAM, (uintptr_t)Immf, sizeof(*Immf));
    wires_ptr.insert(std::make_pair(ImmTrue, Immt));
    wires_ptr.insert(std::make_pair(ImmFalse, Immf));
    
    wires.push_back(std::make_pair(ImmTrue, handle_T));
    wires.push_back(std::make_pair(ImmFalse, handle_F));
    int arg_counter = 0;
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        for(int i = 0; i < it->second.first; i++){
            auto handle = new starpu_data_handle_t;
            t_val* arg = (t_val*)calloc(1, sizeof(t_val));
            *arg = arg_in[arg_counter];
            starpu_variable_data_register(handle, STARPU_MAIN_RAM, (uintptr_t)arg, sizeof(t_val));
            wires.push_back(std::make_pair(it->second.second[i], handle));
            wires_ptr.insert(std::make_pair(it->second.second[i], arg));
            //std::cout << "input '" << it->first << "[" << i << "] : " << (*arg) << std::endl;
            arg_counter++;
        }
    }
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        //std::cout << "FF   :" << FF->first << " " << FF->second->num_outputs << "bit" << std::endl;
        starpu_data_handle_t *handle = new starpu_data_handle_t;
        FF->second.first->dff_mem = (uintptr_t)handle;
        starpu_variable_data_register(handle, STARPU_MAIN_RAM, (uintptr_t)FF->second.second, sizeof(t_val) * num_mems);
        wires.push_back(std::make_pair(FF->second.first->outputs[0], (starpu_data_handle_t*)FF->second.first->dff_mem));
        wires_ptr.insert(std::make_pair(FF->second.first->outputs[0], (t_val*)FF->second.second));
    }

    
    wire* next;
    starpu_data_handle_t* next_handle;
    for(int i = 0; i < wires.size(); i++)
    {
        next = wires[i].first;
        next_handle = wires[i].second;
        auto newqueue = new std::queue<node*>;
        node* nextnode;
        while (!next->dep->empty())
        {
            nextnode = next->dep->front();
            for(int i = 0; i < nextnode->num_inputs; i++){
                if(nextnode->inputs[i].second == next){
                    nextnode->inputs[i].first = next_handle;
                    nextnode->d_counter--;
                }
            }
            if((nextnode->d_counter == 0) && !(nextnode->type->isFF)){
                auto handle_out = new starpu_data_handle_t[nextnode->num_outputs];
                auto wire_descrs = (struct starpu_data_descr*)calloc(nextnode->num_inputs + nextnode->num_outputs, sizeof(struct starpu_data_descr));
                for(int i = 0; i < nextnode->num_inputs; i++){
                    wire_descrs[i].handle = *(starpu_data_handle_t*)(nextnode->inputs[i].first);
                    wire_descrs[i].mode = STARPU_R;
                }
                for(int i = 0; i < nextnode->num_outputs; i++){
                    t_val* out_b = (t_val*)calloc(1, sizeof(t_val));
                    starpu_variable_data_register(&handle_out[i], STARPU_MAIN_RAM, (uintptr_t)out_b , sizeof(*out_b));
                    wires.push_back(std::make_pair(nextnode->outputs[i], &(handle_out[i])));
                    wires_ptr.insert(std::make_pair(nextnode->outputs[i],out_b));
                    wire_descrs[i + nextnode->num_inputs].handle = handle_out[i];
                    wire_descrs[i + nextnode->num_inputs].mode = STARPU_RW;
                }
                starpu_task_insert((starpu_codelet*)nextnode->type->cl,
                    STARPU_DATA_MODE_ARRAY, wire_descrs, nextnode->num_inputs + nextnode->num_outputs,
                    0);
                nextnode->d_counter = nextnode->num_inputs;
            } 
            newqueue->push(next->dep->front());
            next->dep->pop();
        }
        delete next->dep;
        next->dep = newqueue;
    }
    for(auto it = outputs.begin() ; it != outputs.end(); it++){
        for(int j = 0; j < it->second.first; j++){
            if(wires_ptr.find(it->second.second[j]) == wires_ptr.end()) std::cout << "aaa" << std::endl;
            output_handles[it->first].second[j] = wires_ptr[it->second.second[j]];
        }
    }
#ifndef use_simple_FF
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        node* FF_node = FF->second.first;
        auto wire_descrs = (struct starpu_data_descr*)calloc(FF_node->num_inputs + 1, sizeof(struct starpu_data_descr));
        for(int i = 0; i < FF_node->num_inputs; i++){
            wire_descrs[i].handle = *(starpu_data_handle_t*)(FF_node->inputs[i].first);
            wire_descrs[i].mode = STARPU_R;
        }
        wire_descrs[FF->second.first->num_inputs].handle = *(starpu_data_handle_t*)(FF->second.first->dff_mem);
        wire_descrs[FF->second.first->num_inputs].mode = STARPU_RW;
        
        starpu_task_insert((starpu_codelet*)FF_node->type->cl,
            STARPU_DATA_MODE_ARRAY, wire_descrs, FF_node->num_inputs + FF_node->num_outputs,
            0);
    }
#endif

    starpu_task_wait_for_all();
#ifdef use_simple_FF
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        *(t_val*)FF->second.second = *(t_val*)wires_ptr[FF->second.first->inputs[0].second];
    }
#endif
    for(auto it = output_handles.begin(); it != output_handles.end(); it++){
        t_val** ret = (t_val**)it->second.second;
        for(int i = 0; i < it->second.first; i++){
        retvals->push_back(*ret[i]);
        }
    }
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){ //avoid deletion of internal datam of FF
        auto it = wires_ptr.find(FF->second.first->outputs[0]);
        wires_ptr.erase(it);
    }
    for(auto it = wires_ptr.begin(); it != wires_ptr.end(); it++){ //free all temp datam and data_handles
        free((t_val*)it->second);
    }
    starpu_task_wait_for_all();
    for(auto next : wires){
        free(next.second);
    }
    return 0;
}


//return a vector of inputs for "insert_***_mpi" functions
std::vector<int>* reg_handles(
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
        wire_index++;
    }
    t_val* Immt = (t_val*)malloc(sizeof(t_val));
    t_val* Immf = (t_val*)malloc(sizeof(t_val));
#ifdef plain_mode
    *Immt = true;
    *Immf = false;
#else
    TFHEpp::HomCONSTANTONE<lvl_param>(*Immt);
    TFHEpp::HomCONSTANTZERO<lvl_param>(*Immf);
#endif

    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)Immt, sizeof(*Immt));
    wires_ptr.insert(std::make_pair(ImmTrue, Immt));
    wires.insert(std::make_pair(ImmTrue, wire_index));
    if(ImmTrue->output_id >= 0){
        retval_handle_id[ImmTrue->output_id] = wire_index;
        retval_ptrs[ImmTrue->output_id] = (Immt);
    }
    wire_index++;

    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)Immf, sizeof(*Immf));
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
                for(int i = 0; i < nextnode->num_inputs; i++) tasks->push_back((int)(intptr_t)nextnode->inputs[i].first);
                for(int i = 0; i < nextnode->num_outputs; i++){
                    t_val* out_b = (t_val*)calloc(1, sizeof(t_val));
                    starpu_variable_data_register(&wire_handles[wire_index], STARPU_MAIN_RAM, (uintptr_t)out_b , sizeof(t_val));
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

int insert_tasks(
    std::vector<int>* tasks,
    starpu_data_handle_t* wire_handles
){

    int task_index = 0;
    int task_id = 0;
    while(task_index < tasks->size()){
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
    return 0;
}