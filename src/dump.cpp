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

    if(!argv[1]){
        std::cerr << "input required!" << std::endl;
        return 1;
    }
    auto inputs_ = new std::map<std::string, int>;
    auto outputs_ = new std::map<std::string, int>; 
    getio(std::string(argv[1]), inputs_, outputs_);
#ifdef plain_mode
    auto arg_in = make_inputs_plain_manual(*inputs_);
#else
    std::unique_ptr<TFHEpp::SecretKey> sk(new TFHEpp::SecretKey);
    ek.emplacebkfft<TFHEpp::lvl01param>(*sk);
    ek.emplaceiksk<TFHEpp::lvl10param>(*sk);
    auto arg_in = make_inputs_cipher_rand(*inputs_, *sk);
#endif
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
#ifdef plain_mode
    auto retvals = deploygates_plain(nodetypes, *inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump_plain(*outputs_, retvals);
#else
    auto retvals = deploygates_cipher(nodetypes, *inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump_cipher(*outputs_, retvals, *sk);
#endif

#ifdef plain_mode
    arg_in = make_inputs_plain_manual(*inputs_);
#else
    arg_in = make_inputs_cipher_manual(*inputs_, *sk);
#endif
#ifdef plain_mode
    retvals = deploygates_plain(nodetypes, *inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump_plain(*outputs_, retvals);
#else
    retvals = deploygates_cipher(nodetypes, *inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump_cipher(*outputs_, retvals, *sk);
#endif
}
