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


TFHEpp::EvalKey ek;

int main(int argc, char** argv){
    starpu_init(NULL);

    std::unique_ptr<TFHEpp::SecretKey> sk(new TFHEpp::SecretKey);
    ek.emplacebkfft<TFHEpp::lvl01param>(*sk);
    ek.emplaceiksk<TFHEpp::lvl10param>(*sk);

    std::map<std::string, nodetype*> nodetypes = types_init();
    node andnode;
    andnode.inputs = (std::pair<void*, wire*>*)malloc(2*sizeof(std::pair<TFHEpp::TLWE<TFHEpp::lvl1param>*, wire*>));
    std::vector<uint8_t> plain1 = {1};
    auto arg1 = TFHEpp::bootsSymEncrypt(plain1, *sk);
    starpu_data_handle_t handle1;
    starpu_variable_data_register(&handle1, STARPU_MAIN_RAM, (uintptr_t)&arg1[0], sizeof(arg1[0]));
    std::vector<uint8_t> plain2 = {1};
    auto arg2 = TFHEpp::bootsSymEncrypt(plain2, *sk);
    starpu_data_handle_t handle2;
    starpu_variable_data_register(&handle2, STARPU_MAIN_RAM, (uintptr_t)&arg2[0], sizeof(arg2[0]));
    andnode.inputs[0].first = &handle1;
    andnode.inputs[1].first = &handle2;
    andnode.type = nodetypes["$_AND_"];
    auto ret1 = new TFHEpp::TLWE<TFHEpp::lvl1param>;
    starpu_data_handle_t handle_ret;
    starpu_variable_data_register(&handle_ret, STARPU_MAIN_RAM, (uintptr_t)ret1, sizeof(*ret1));
    reinterpret_cast<void (*)(node, starpu_data_handle_t*)>(andnode.type->task_insert)(andnode, &handle_ret);
    starpu_task_wait_for_all();
    std::vector<TFHEpp::TLWE<TFHEpp::lvl1param>> retvals{};
    retvals.push_back(*ret1);
    std::vector<uint8_t> result = bootsSymDecrypt(retvals, *sk);
    std::cout << (result[0] ? "true" : "false") << std::endl;
    
}
