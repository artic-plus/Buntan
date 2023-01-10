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
            TFHEpp::HomCONSTANTZERO(FF->second.second[i]);
#endif
    }
    return 0;
}




std::vector<t_val> make_inputs_rand(
    std::map<std::string,int> inputs
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> plain{};
#else
    std::vector<uint8_t> plain{};
#endif
    std::random_device rnd;
    int r = 0;
    for(int t = 0; t < n; t++){
        for(auto it = inputs.begin(); it != inputs.end(); it++){
            int w = it->second;
            for(int i = 0; i < w;i++){
                if(r == 0) {
                    r = rnd();
                }
                plain.push_back((r & 1));
                std::cout << "input '" << it->first << "[" << i << "] : " << (r & 1) << std::endl;
                r = r >> 1;
            }
        }
    }
#ifdef plain_mode
    return plain;
#else
    return TFHEpp::bootsSymEncrypt(plain, sk);
#endif
}

std::vector<t_val> make_inputs_manual(
    std::map<std::string,int> inputs
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> plain{};
#else
    std::vector<uint8_t> plain{};
#endif
    for(int t = 0; t < n; t++){
        for(auto it = inputs.begin(); it != inputs.end(); it++){
            int w = it->second;
            std::cout << "input '" << it->first << "[" << w << "]" << std::endl;
            int bits;
            std::cin >> bits;
            if(std::cin.fail()){
                std::cerr << "input error" << std::endl;
                return std::vector<t_val>{};
            }
            for(int i = 0; i < w;i++){
                plain.push_back((bits & 1));
                bits = bits >> 1;
            }
        }
    }
#ifdef plain_mode
    return plain;
#else
    return TFHEpp::bootsSymEncrypt(plain, sk);
#endif
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
    TFHEpp::HomCONSTANTONE(*Immt);
    TFHEpp::HomCONSTANTZERO(*Immf);
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
        starpu_data_handle_t *handle = new starpu_data_handle_t[num_mems];
        FF->second.first->dff_mem = (uintptr_t)handle;
        for(int j = 0; j < num_mems; j++){
            starpu_variable_data_register(&(handle[j]), STARPU_MAIN_RAM, (uintptr_t)&(FF->second.second[j]), sizeof(t_val));
        }
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
                t_val* out_b = (t_val*)calloc(nextnode->num_outputs, sizeof(t_val));
                for(int i = 0; i < nextnode->num_outputs; i++){
                    starpu_variable_data_register(&handle_out[i], STARPU_MAIN_RAM, (uintptr_t)&(out_b[i]) , sizeof(out_b[i]));
                    wires.push_back(std::make_pair(nextnode->outputs[i], &(handle_out[i])));
                    wires_ptr.insert(std::make_pair(nextnode->outputs[i],out_b));
                }
                reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(nextnode->type->task_insert)(*nextnode, handle_out);
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
        reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(FF->second.first->type->task_insert)(*(FF->second.first), (starpu_data_handle_t*)nullptr);
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


int result_dump(
    std::map<std::string,int> outputs,
    std::vector<t_val> retvals
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> result = retvals;
#else
    std::vector<uint8_t> result = bootsSymDecrypt(retvals, sk);
#endif
    int ret_counter = 0;
    for(int t = 0; t < n; t++){
        if(n > 1) std::cout << "CLK : " << t+1 << std::endl;
        for(auto it = outputs.begin(); it != outputs.end(); it++){
            for(int i = 0; i < it->second; i++){
                std::cout << "output '" << it->first <<"[" << i << "] : " << (result[ret_counter] ? "true": "false") << std::endl;
                ret_counter++;
                if(ret_counter > retvals.size()){
                    std::cerr << "err: too few retvals!" << std::endl;
                    return 1;
                }
            }
        }
    }
    return 0;
}
