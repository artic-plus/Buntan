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
    auto arg_in = make_inputs_manual(*inputs_);
#else
    std::unique_ptr<TFHEpp::SecretKey> sk(new TFHEpp::SecretKey);
    ek.emplacebkfft<TFHEpp::lvl01param>(*sk);
    ek.emplaceiksk<TFHEpp::lvl10param>(*sk);
    auto arg_in = make_inputs_rand(*inputs_, *sk);
#endif
    auto inputs = new std::map<std::string, std::pair<int, wire**>>;
    auto outputs = new std::map<std::string, std::pair<int, wire**>>; 
    auto FFs = new std::map<std::string, std::pair<node*, t_val*>>;
    wire* ImmTrue = new wire{nullptr, new std::queue<node*>, false}; 
    wire* ImmFalse = new wire{nullptr, new std::queue<node*>, false};
    std::map<std::string, nodetype*> nodetypes = types_init();
    if(yosys_json_parser(std::string(argv[1]), nodetypes, inputs, outputs, FFs, ImmTrue, ImmFalse)){
        std::cerr << "frontend failed!" << std::endl;
        return 1;
    }
    starpu_init(NULL);
    init_FFs(FFs);
    //checkgraph(nodetypes, *inputs, *outputs, *FFs, ImmTrue, ImmFalse);
#ifdef plain_mode
    auto retvals = deploygates(*inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump(*outputs_, retvals);
#else
    auto retvals = deploygates(*inputs, arg_in, *outputs, *FFs, ImmTrue, ImmFalse);
    result_dump(*outputs_, retvals, *sk);
#endif

}
