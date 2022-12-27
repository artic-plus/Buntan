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
    result_dump(*outputs, result);
#else
    TFHEpp::SecretKey sk;
    {
        std::ifstream ifs{"secret.key", std::ios::binary};
        cereal::PortableBinaryInputArchive ar(ifs);
        sk.serialize(ar);
    };
    result_dump(*outputs, result, sk);
#endif
}