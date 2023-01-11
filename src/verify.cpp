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
    std::string filepath = "./circuit.json";
    int n = 1;
    for(int i = 0; i < argc - 1; i++){
        if(!strcmp(argv[i], "--circuit"))
            filepath = std::string(argv[i+1]);
        if(!strcmp(argv[i], "--repetition"))
            n = atoi(argv[i+1]);
    }
    auto inputs = new std::map<std::string, int>;
    auto outputs = new std::map<std::string, int>; 
    if(getio(filepath, inputs, outputs)) return 1;
#ifdef plain_mode
    std::vector<bool> result{};
#else
    std::vector<TFHEpp::TLWE<TFHEpp::lvl1param>> result{};
#endif
    {
        std::ifstream ifs("./result.data", std::ios::binary);
        cereal::PortableBinaryInputArchive ar(ifs);
        ar(result);
    }
#ifdef plain_mode
    result_dump(*outputs, result, n);
#else
    TFHEpp::SecretKey sk;
    {
        std::ifstream ifs{"secret.key", std::ios::binary};
        cereal::PortableBinaryInputArchive ar(ifs);
        sk.serialize(ar);
    };
    result_dump(*outputs, result, sk, n);
#endif
}