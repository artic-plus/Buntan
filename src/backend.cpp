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



int init_FFs(std::map<std::string, std::pair<node*, uintptr_t>>* FFs){
    int num_mems = 3; // for state, fCLK, and fRST
    for(auto it = FFs->begin(); it != FFs->end(); it++){
        starpu_data_handle_t *handle = new starpu_data_handle_t[num_mems];
#ifdef plain_mode
        bool* mem = (bool*)calloc(num_mems, sizeof(bool));
#else
        int* mem;
#endif
        for(int j = 0; j < num_mems; j++){
            starpu_variable_data_register(&(handle[j]), STARPU_MAIN_RAM, (uintptr_t)&(mem[j]), sizeof(mem[j]));
        }
        it->second.first->dff_mem = (uintptr_t)handle;
        it->second.second = (uintptr_t)mem;
    }
    return 0;
}


std::vector<bool> make_inputs_plain_rand(std::map<std::string,int> inputs){
    std::vector<bool> input_bits{};
    std::random_device rnd;
    int r = 0;
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        int w = it->second;
        for(int i = 0; i < w;i++){
            if(r == 0) {
                r = rnd();
            }
            input_bits.push_back((r & 1));
            std::cout << "input '" << it->first << "[" << i << "] : " << (r & 1) << std::endl;
            r = r >> 1;
        }
    }
    return input_bits;
}

std::vector<bool> make_inputs_plain_manual(std::map<std::string,int> inputs){
    std::vector<bool> input_bits{};
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        int w = it->second;
        std::cout << "input '" << it->first << "[" << w << "]" << std::endl;
        int bits;
        std::cin >> bits;
        if(std::cin.fail()){
            std::cerr << "input error" << std::endl;
            return std::vector<bool>{};
        }
        for(int i = 0; i < w;i++){
            input_bits.push_back((bits & 1));
            bits = bits >> 1;
        }
    }
    return input_bits;
}


std::vector<bool> deploygates_plain(
    std::map<std::string, nodetype*> nodetypes, 
    std::map<std::string, std::pair<int, wire**>> inputs,
    std::vector<bool> arg_in,
    std::map<std::string, std::pair<int, wire**>> outputs, 
    std::map<std::string, std::pair<node*, uintptr_t>> FFs, 
    wire* ImmTrue, 
    wire* ImmFalse
){
    std::queue<std::pair<wire*, starpu_data_handle_t*>> wires;
    std::map<wire*, uintptr_t> wires_ptr;

    std::map<std::string, std::pair<int, uintptr_t*>> output_handles; 
    for(auto it = outputs.begin() ; it != outputs.end(); it++){
        uintptr_t* ptrs = (uintptr_t*)calloc(it->second.first, sizeof(uintptr_t));
        output_handles.insert(std::make_pair(it->first, std::make_pair(it->second.first, ptrs)));
    }

    bool* Immt = (bool*)malloc(sizeof(bool));
    *Immt = true;
    bool* Immf = (bool*)malloc(sizeof(bool));
    *Immf = false;

    auto handle_T = new starpu_data_handle_t;
    auto handle_F = new starpu_data_handle_t;
    starpu_variable_data_register(handle_T, STARPU_MAIN_RAM, (uintptr_t)Immt, sizeof(*Immt));
    starpu_variable_data_register(handle_F, STARPU_MAIN_RAM, (uintptr_t)Immf, sizeof(*Immf));
    wires_ptr.insert(std::make_pair(ImmTrue, (uintptr_t)Immt));
    wires_ptr.insert(std::make_pair(ImmFalse, (uintptr_t)Immf));
    
    wires.push(std::make_pair(ImmTrue, handle_T));
    wires.push(std::make_pair(ImmFalse, handle_F));
    int arg_counter = 0;
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        for(int i = 0; i < it->second.first; i++){
            auto handle = new starpu_data_handle_t;
            bool* arg = (bool*)calloc(1, sizeof(bool));
            *arg = arg_in[arg_counter];
            starpu_variable_data_register(handle, STARPU_MAIN_RAM, (uintptr_t)arg, sizeof(bool));
            wires.push(std::make_pair(it->second.second[i], handle));
            wires_ptr.insert(std::make_pair(it->second.second[i], (uintptr_t)arg));
            //std::cout << "input '" << it->first << "[" << i << "] : " << (*arg) << std::endl;
            arg_counter++;
            if(arg_counter > arg_in.size()){
                std::cerr << "err: too few arguments" << std::endl;
                return std::vector<bool>{};
            }
        }
    }
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        //std::cout << "FF   :" << FF->first << " " << FF->second->num_outputs << "bit" << std::endl;
        wires.push(std::make_pair(FF->second.first->outputs[0], (starpu_data_handle_t*)FF->second.first->dff_mem));
        wires_ptr.insert(std::make_pair(FF->second.first->outputs[0], FF->second.second));
    }

    
    wire* next;
    starpu_data_handle_t* next_handle;
    while (!wires.empty())
    {
        next = wires.front().first;
        next_handle = wires.front().second;
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
                for(int i = 0; i < nextnode->num_outputs; i++){
                    bool* out_b = (bool*)calloc(1, sizeof(bool) * nextnode->num_outputs);
                    starpu_variable_data_register(&handle_out[i], STARPU_MAIN_RAM, (uintptr_t)&(out_b[i]) , sizeof(out_b[i]));
                    wires.push(std::make_pair(nextnode->outputs[i], &(handle_out[i])));
                    wires_ptr.insert(std::make_pair(nextnode->outputs[i],(uintptr_t)out_b));
                }
                reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(nextnode->type->task_insert)(*nextnode, handle_out);
                nextnode->d_counter = nextnode->num_inputs;
            } 
            newqueue->push(next->dep->front());
            next->dep->pop();
        }
        delete next->dep;
        next->dep = newqueue;
        wires.pop();
    }
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(FF->second.first->type->task_insert)(*(FF->second.first), (starpu_data_handle_t*)nullptr);
    }
    for(auto it = outputs.begin() ; it != outputs.end(); it++){
        for(int j = 0; j < it->second.first; j++){
            output_handles[it->first].second[j] = wires_ptr[it->second.second[j]];
        }
    }
    starpu_task_wait_for_all();
    std::vector<bool> retvals{};
    for(auto it = output_handles.begin(); it != output_handles.end(); it++){
        bool** ret = (bool**)it->second.second;
        for(int i = 0; i < it->second.first; i++){
        //std::cout << "output '" << it->first <<"[" << i << "] : " << (*ret[i] ? "true": "false") << std::endl;
        retvals.push_back(*ret[i]);
        }
        
    }
    return retvals;
}

int result_dump(
    std::map<std::string,int> outputs,
    std::vector<bool> retvals
){
    int ret_counter = 0;
    for(auto it = outputs.begin(); it != outputs.end(); it++){
        for(int i = 0; i < it->second; i++){
            std::cout << "output '" << it->first <<"[" << i << "] : " << (retvals[ret_counter] ? "true": "false") << std::endl;
            ret_counter++;
            if(ret_counter > retvals.size()){
                std::cerr << "err: too few retvals!" << std::endl;
                return 1;
            }
        }
    }
    return 0;
}