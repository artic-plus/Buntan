#include <starpu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "frontend.hpp"
#include "nodetypes.hpp"


uintptr_t* init_FFs(std::map<std::string, node*>* FFs);
int deploygates(std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>> inputs, std::map<std::string, std::pair<int, wire**>> outputs, std::map<std::string, node*> FFs, uintptr_t* FF_ptr, wire* ImmTrue, wire* ImmFalse);

int main(int argc, char** argv){

    if(!argv[1]){
        std::cerr << "input required!" << std::endl;
        return 1;
    }
    auto inputs = new std::map<std::string, std::pair<int, wire**>>;
    auto outputs = new std::map<std::string, std::pair<int, wire**>>; 
    auto FFs = new std::map<std::string, node*>;
    wire* ImmTrue = new wire{nullptr, new std::queue<node*>, false}; 
    wire* ImmFalse = new wire{nullptr, new std::queue<node*>, false};
    std::map<std::string, nodetype*> nodetypes = types_init();
    if(yosys_json_parser(std::string(argv[1]), nodetypes, inputs, outputs, FFs, ImmTrue, ImmFalse)){
        std::cerr << "frontend failed!" << std::endl;
        return 1;
    }
    starpu_init(NULL);
    uintptr_t* FF_ptr = init_FFs(FFs);
    shownodes(nodetypes, *inputs, *outputs, *FFs, ImmTrue, ImmFalse);
    deploygates(nodetypes, *inputs, *outputs, *FFs, FF_ptr, ImmTrue, ImmFalse);
}


uintptr_t* init_FFs(std::map<std::string, node*>* FFs){
    int num_mems = 3; // for state, fCLK, and fRST
    int i = 0;
    uintptr_t* FF_ptr = (uintptr_t*)calloc(FFs->size(), sizeof(uintptr_t*));
    for(auto it = FFs->begin(); it != FFs->end(); it++){
        starpu_data_handle_t *handle = new starpu_data_handle_t[num_mems];
#ifdef plain_mode
        bool* mem = (bool*)calloc(num_mems, sizeof(bool));
#else
        int* mem;
#endif
        for(int j = 0; j < num_mems; j++){
            starpu_variable_data_register(&(handle[i]), STARPU_MAIN_RAM, (uintptr_t)mem[j], sizeof(mem[j]));
        }
        it->second->dff_mem = handle;
        FF_ptr[i] = (uintptr_t)mem;
        i++;
    }
    return (uintptr_t*)FF_ptr;
}



int deploygates(
    std::map<std::string, nodetype*> nodetypes, 
    std::map<std::string, std::pair<int, wire**>> inputs, 
    std::map<std::string, std::pair<int, wire**>> outputs, 
    std::map<std::string, node*> FFs, 
    uintptr_t* FF_ptr,
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

#ifdef plain_mode
    bool* Immt = (bool*)malloc(sizeof(bool));
    *Immt = true;
    bool* Immf = (bool*)malloc(sizeof(bool));
    *Immf = false;
#else
    int* Immt;
    int* Immf;
#endif

    auto handle_T = new starpu_data_handle_t;
    auto handle_F = new starpu_data_handle_t;
    starpu_variable_data_register(handle_T, STARPU_MAIN_RAM, (uintptr_t)Immt, sizeof(*Immt));
    starpu_variable_data_register(handle_F, STARPU_MAIN_RAM, (uintptr_t)Immf, sizeof(*Immf));
    wires_ptr.insert(std::make_pair(ImmTrue, (uintptr_t)Immt));
    wires_ptr.insert(std::make_pair(ImmFalse, (uintptr_t)Immf));
    
    wires.push(std::make_pair(ImmTrue, handle_T));
    wires.push(std::make_pair(ImmFalse, handle_F));
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        for(int i = 0; i < it->second.first; i++){
#ifdef plain_mode
            bool* in_b = (bool*)malloc(sizeof(bool));
            *in_b = true; //
#else
            int* in_b;
#endif
            auto handle = new starpu_data_handle_t;
            starpu_variable_data_register(handle, STARPU_MAIN_RAM, (uintptr_t)in_b, sizeof(*in_b));
            wires.push(std::make_pair(it->second.second[i], handle));
            wires_ptr.insert(std::make_pair(it->second.second[i], (uintptr_t)in_b));
        }
    }
    int FF_count = 0;
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        //std::cout << "FF   :" << FF->first << " " << FF->second->num_outputs << "bit" << std::endl;
        wires.push(std::make_pair(FF->second->outputs[0], (starpu_data_handle_t*)FF->second->dff_mem));
        wires_ptr.insert(std::make_pair(FF->second->outputs[0], FF_ptr[FF_count]));
        FF_count++;
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
#ifdef plain_mode
                    bool* out_b = (bool*)calloc(1, sizeof(bool) * nextnode->num_outputs);
#else
                    int* out_b;
#endif
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
    for(auto it = outputs.begin() ; it != outputs.end(); it++){
        for(int j = 0; j < it->second.first; j++){
            output_handles[it->first].second[j] = wires_ptr[it->second.second[j]];
        }
    }
    std::cout << "hoge" << std::endl;
    starpu_task_wait_for_all();
    for(auto it = output_handles.begin(); it != output_handles.end(); it++){
#ifdef plain_mode
        bool** ret = (bool**)it->second.second;
        for(int i = 0; i < it->second.first; i++){
        std::cout << "output '" << it->first <<"[" << i << "] : " << (*ret[i] ? "true": "false") << std::endl;
        }
#else
#endif
        
    }
    return 0;   
}