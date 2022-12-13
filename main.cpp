//#include <starpu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "frontend.hpp"
#include "nodetypes.hpp"


void* and_cl;
void* nand_cl;
void* andnot_cl;
void* or_cl;
void* nor_cl;
void* ornot_cl;
void* xor_cl;
void* xnor_cl;
void* not_cl;
void* dff_p_cl;
void* dff_n_cl;
void* adff_pp1_cl;
void* adff_pp0_cl;
void* dffe_pp_cl;
void* mux_cl;
void* nmux_cl;
void* fa_cl;


int init_cl(){
    and_cl = nullptr;
    nand_cl = nullptr;
    andnot_cl = nullptr;
    or_cl = nullptr;
    nor_cl = nullptr;
    ornot_cl = nullptr;
    xor_cl = nullptr;
    xnor_cl = nullptr;
    not_cl = nullptr;
    dff_p_cl = nullptr;
    dff_n_cl = nullptr;
    adff_pp1_cl = nullptr;
    adff_pp0_cl = nullptr;
    dffe_pp_cl = nullptr;
    mux_cl = nullptr;
    nmux_cl = nullptr;
    fa_cl = nullptr;
    return 0;
}

int main(int argc, char** argv){

    auto inputs = new std::map<std::string, std::pair<int, wire**>>;
    auto outputs = new std::map<std::string, std::pair<int, wire**>>; 
    auto FFs = new std::map<std::string, node*>;
    wire* ImmTrue = new wire{nullptr, new std::queue<node*>, false}; 
    wire* ImmFalse = new wire{nullptr, new std::queue<node*>, false};
    init_cl();
    std::map<std::string, nodetype*> nodetypes = types_init();
    if(yosys_json_parser(std::string(argv[1]), nodetypes, inputs, outputs, FFs, ImmTrue, ImmFalse)){
        std::cerr << "frontend failed!" << std::endl;
    }
    shownodes(nodetypes, *inputs, *outputs, *FFs, ImmTrue, ImmFalse);
}