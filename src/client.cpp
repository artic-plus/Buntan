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
    auto arg_in = make_inputs_rand(*inputs, n);
#else
    evalkey ek;
    auto sk = gen_key(ek);
    // export the secret key to file for later use
    {
        std::ofstream ofs{"secret.key", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        sk->serialize(ar);
    };

    // export the cloud key to a file (for the cloud)
    {
        std::ofstream ofs{"cloud.key", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        ek.serialize(ar);
    };
    auto arg_in = make_inputs_rand(*inputs, *sk, n);
#endif
    {
        std::ofstream ofs{"cloud.data", std::ios::binary};
        cereal::PortableBinaryOutputArchive ar(ofs);
        ar(arg_in);
    };
}
