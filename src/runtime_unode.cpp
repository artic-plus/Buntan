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


evalkey ek;

int main(int argc, char** argv){
    std::string filepath = "./circuit.json";
    int n = 1;
    for(int i = 0; i < argc - 1; i++){
        if(!strcmp(argv[i], "--circuit"))
            filepath = std::string(argv[i+1]);
        if(!strcmp(argv[i], "--repetition"))
            n = atoi(argv[i+1]);
    }
    auto inputs = new std::map<std::string, std::pair<int, wire**>>;
    auto outputs = new std::map<std::string, std::pair<int, wire**>>; 
    auto FFs = new std::map<std::string, std::pair<node*, t_val*>>;
    wire* ImmTrue = new wire{nullptr, new std::queue<node*>, false, -1}; 
    wire* ImmFalse = new wire{nullptr, new std::queue<node*>, false, -1};
#ifdef plain_mode
    types_init(1);
#else
    types_init(0);
#endif
    int numwires[4];
    yosys_json_parser(filepath, numwires, inputs, outputs, FFs, ImmTrue, ImmFalse);
    if(numwires[0] < 0){
        std::cerr << "frontend failed!" << std::endl;
        return 1;
    }
    starpu_init(NULL);
    if(FFs->size() > 0)init_FFs(FFs);
    checkgraph(*inputs, *outputs, *FFs, ImmTrue, ImmFalse);
#ifndef plain_mode
    {
        const std::string path = "./cloud.key";
        std::ifstream ifs("./cloud.key", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        (&ek)->serialize(ar);
    }
#endif
    std::vector<t_val> args_in{};
    {
        std::ifstream ifs("./cloud.data", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        ar(args_in);
    }
    auto retvals = new std::vector<t_val>{};
    t_val args_in_arr[numwires[1]];
    for(int t = 0; t < n; t++){
        std::copy(args_in.begin() + t * numwires[1], args_in.begin() + (t + 1) * numwires[1], args_in_arr);
        deploygates(*inputs, args_in_arr, *outputs, retvals, *FFs, ImmTrue, ImmFalse);
    }
    {
        std::ofstream ofs{"result.data", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        ar(*retvals);
    };
}
