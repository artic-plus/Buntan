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
#include "backend.hpp"
#include "nodetypes.hpp"

int main(int argc, char** argv){

    if(!argv[1]){
        std::cerr << "input required!" << std::endl;
        return 1;
    }
    auto inputs = new std::map<std::string, int>;
    auto outputs = new std::map<std::string, int>; 
    getio(std::string(argv[1]), inputs, outputs);
    std::vector<bool> plain_out{};
    {
        std::ifstream ifs("./result.data", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        ar(plain_out);
    }
    result_dump(*outputs, plain_out);
}