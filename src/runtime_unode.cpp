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

int main(int argc, char** argv){

    if(!argv[1]){
        std::cerr << "input required!" << std::endl;
        return 1;
    }
    auto inputs = new std::map<std::string, std::pair<int, wire**>>;
    auto outputs = new std::map<std::string, std::pair<int, wire**>>; 
    auto FFs = new std::map<std::string, std::pair<node*, uintptr_t>>;
    wire* ImmTrue = new wire{nullptr, new std::queue<node*>, false}; 
    wire* ImmFalse = new wire{nullptr, new std::queue<node*>, false};
    std::map<std::string, nodetype*> nodetypes = types_init();
    if(yosys_json_parser(std::string(argv[1]), nodetypes, inputs, outputs, FFs, ImmTrue, ImmFalse)){
        std::cerr << "frontend failed!" << std::endl;
        return 1;
    }
    starpu_init(NULL);
    init_FFs(FFs);
    checkgraph(nodetypes, *inputs, *outputs, *FFs, ImmTrue, ImmFalse);
    std::vector<bool> plain_in{};
    {
        std::ifstream ifs("./cloud.data", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        ar(plain_in);
    }
    auto retvals = deploygates_plain(nodetypes, *inputs, plain_in, *outputs, *FFs, ImmTrue, ImmFalse);
    {
        std::ofstream ofs{"result.data", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        ar(retvals);
    };
}
