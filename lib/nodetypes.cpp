
#include <string>
#include <vector>
#include <map>
#include <starpu.h>
//#include <starpu_mpi.h>

#include "nodetypes.hpp"
#include <tfhe++.hpp>


void c_one(bool_enc &one){
        TFHEpp::HomCONSTANTONE<lvl_param>(one);
}
void c_zero(bool_enc &zero){
        TFHEpp::HomCONSTANTZERO<lvl_param>(zero);
}

extern struct starpu_codelet and_p_cl;
extern struct starpu_codelet and_c_cl;
extern struct starpu_codelet nand_p_cl;
extern struct starpu_codelet nand_c_cl;
extern struct starpu_codelet andnot_p_cl;
extern struct starpu_codelet andnot_c_cl;
extern struct starpu_codelet or_p_cl;
extern struct starpu_codelet or_c_cl;
extern struct starpu_codelet nor_p_cl;
extern struct starpu_codelet nor_c_cl;
extern struct starpu_codelet ornot_p_cl;
extern struct starpu_codelet ornot_c_cl;
extern struct starpu_codelet xor_p_cl;
extern struct starpu_codelet xor_c_cl;
extern struct starpu_codelet xnor_p_cl;
extern struct starpu_codelet xnor_c_cl;
extern struct starpu_codelet not_p_cl;
extern struct starpu_codelet not_c_cl;
extern struct starpu_codelet dff_p_p_cl;
extern struct starpu_codelet dff_p_c_cl;
extern struct starpu_codelet dff_n_p_cl;
extern struct starpu_codelet dff_n_c_cl;
extern struct starpu_codelet adff_pp1_p_cl;
extern struct starpu_codelet adff_pp1_c_cl;
extern struct starpu_codelet adff_pp0_p_cl;
extern struct starpu_codelet adff_pp0_c_cl;
extern struct starpu_codelet dffe_pp_p_cl;
extern struct starpu_codelet dffe_pp_c_cl;
extern struct starpu_codelet mux_p_cl;
extern struct starpu_codelet mux_c_cl;
extern struct starpu_codelet nmux_p_cl;
extern struct starpu_codelet nmux_c_cl;

int my_rank = 0;

int types_init(bool plain){
    nodetype* And;
    nodetype* Nand;
    nodetype* AndNot;
    nodetype* Or;
    nodetype* Nor;
    nodetype* OrNot;
    nodetype* Xor;
    nodetype* Xnor;
    nodetype* Not;
    nodetype* DFF_P; 
    nodetype* DFF_N;
    nodetype* ADFF_PP1;
    nodetype* ADFF_PP0;
    nodetype* DFFE_PP;
    nodetype* MUX;
    nodetype* NMUX;
    if(plain){
        And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_AND_", (void*)&and_p_cl, false, 0};
        Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NAND_", (void*)&nand_p_cl, false, 1};
        AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output),"$_ANDNOT_" , (void*)&andnot_p_cl, false, 2};
        Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_OR_", (void*)&or_p_cl, false, 3};
        Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NOR_", (void*)&nor_p_cl, false, 4};
        OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_ORNOT_", (void*)&ornot_p_cl, false, 5};
        Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XOR_", (void*)&xor_p_cl, false, 6};
        Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XNOR_", (void*)&xnor_p_cl, false, 7};
        Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), "$_NOT_", (void*)&not_p_cl, false, 8};
        DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_P_", (void*)&dff_p_p_cl, true, 9};
        DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_N_", (void*)&dff_n_p_cl, true, 10};
        ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP1_", (void*)&adff_pp1_p_cl, true, 11};
        ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP0_", (void*)&adff_pp0_p_cl, true, 12};
        DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), "$_DFFE_PP_", (void*)&dffe_pp_p_cl, true, 13};
        MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_MUX_", (void*)&mux_p_cl, false, 14};
        NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_NMUX_", (void*)&nmux_p_cl, false, 15};
    } else {
        And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_AND_", (void*)&and_c_cl, false, 0};
        Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NAND_", (void*)&nand_c_cl, false, 1};
        AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output),"$_ANDNOT_" , (void*)&andnot_c_cl, false, 2};
        Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_OR_", (void*)&or_c_cl, false, 3};
        Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NOR_", (void*)&nor_c_cl, false, 4};
        OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_ORNOT_", (void*)&ornot_c_cl, false, 5};
        Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XOR_", (void*)&xor_c_cl, false, 6};
        Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XNOR_", (void*)&xnor_c_cl, false, 7};
        Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), "$_NOT_", (void*)&not_c_cl, false, 8};
        DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_P_", (void*)&dff_p_c_cl, true, 9};
        DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_N_", (void*)&dff_n_c_cl, true, 10};
        ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP1_", (void*)&adff_pp1_c_cl, true, 11};
        ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP0_", (void*)&adff_pp0_c_cl, true, 12};
        DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), "$_DFFE_PP_", (void*)&dffe_pp_c_cl, true, 13};
        MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_MUX_", (void*)&mux_c_cl, false, 14};
        NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_NMUX_", (void*)&nmux_c_cl, false, 15};
    }




	nodetypes = std::map<std::string, nodetype*>{
		{"$_AND_", And},
		{"$_NAND_", Nand},
		{"$_ANDNOT_", AndNot},
		{"$_OR_", Or},
		{"$_NOR_", Nor},
		{"$_ORNOT_", OrNot},
		{"$_XOR_", Xor},
		{"$_XNOR_", Xnor},
		{"$_NOT_", Not},
		{"$_DFF_P_", DFF_P},
		{"$_DFF_N_", DFF_N},
		{"$_DFF_PP0_", ADFF_PP0},
		{"$_DFF_PP1_", ADFF_PP1},
		{"$_DFFE_PP_", DFFE_PP},
		{"$_MUX_", MUX},
		{"$_NMUX_", NMUX}
	};

    type_id = std::map<int, nodetype*>{
        {0, And},
		{1, Nand},
		{2, AndNot},
		{3, Or},
		{4, Nor},
		{5, OrNot},
		{6, Xor},
		{7, Xnor},
		{8, Not},
		{9, DFF_P},
		{10, DFF_N},
		{11, ADFF_PP0},
		{12, ADFF_PP1},
		{13, DFFE_PP},
		{14, MUX},
		{15, NMUX}
    };
	return 0;
}


static struct starpu_perfmodel bnode_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "bnode_p_perf_model"
};

static struct starpu_perfmodel bnode_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "bnode_c_perf_model"
};


static struct starpu_perfmodel unode_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "unode_p_perf_model"
};

static struct starpu_perfmodel unode_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "unode_c_perf_model"
};


static struct starpu_perfmodel dff_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "dff_perf_model"
};

static struct starpu_perfmodel dff_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "dff_c_perf_model"
};


static struct starpu_perfmodel adff_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "adff_p_perf_model"
};

static struct starpu_perfmodel adff_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "adff_c_perf_model"
};


static struct starpu_perfmodel dffe_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "dffe_p_perf_model"
};

static struct starpu_perfmodel dffe_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "dffe_c_perf_model"
};


static struct starpu_perfmodel mux_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "mux_p_perf_model"
};

static struct starpu_perfmodel mux_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "mux_c_perf_model"
};

void and_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & *B;
#ifdef dump_mode
    std::cout << "AND GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void and_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomAND<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "AND GATE rank:"  << my_rank << std::endl;
#endif
}

struct starpu_codelet and_p_cl = {
	.cpu_funcs = {and_plain},
    .cpu_funcs_name = {"and_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet and_c_cl = {
    .cpu_funcs = {and_cipher},
    .cpu_funcs_name = {"and_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};


void nand_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !( *A & *B);
#ifdef dump_mode
    std::cout << "NAND GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}


void nand_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomNAND<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "NAND GATE rank:"  << my_rank << std::endl;
#endif
}

struct starpu_codelet nand_p_cl = {
	.cpu_funcs = {nand_plain},
    .cpu_funcs_name = {"nand_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet nand_c_cl = {
    .cpu_funcs = {nand_cipher},
    .cpu_funcs_name = {"nand_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};



void andnot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & !*B;
#ifdef dump_mode
    std::cout << "ANDNOT GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}


void andnot_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomANDYN<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "ANDNOT GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet andnot_p_cl = {
	.cpu_funcs = {andnot_plain},
    .cpu_funcs_name = {"andnot_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet andnot_c_cl = {
    .cpu_funcs = {andnot_cipher},
    .cpu_funcs_name = {"andnot_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};


void or_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | *B;

#ifdef dump_mode
    std::cout << "OR GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void or_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomOR<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "OR GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet or_p_cl = {
	.cpu_funcs = {or_plain},
    .cpu_funcs_name = {"or_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet or_c_cl = {
    .cpu_funcs = {or_cipher},
    .cpu_funcs_name = {"or_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};



void nor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A | *B);
#ifdef dump_mode
    std::cout << "NOR GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void nor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomNOR<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "NOR GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet nor_p_cl = {
	.cpu_funcs = {nor_plain},
    .cpu_funcs_name = {"nor_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet nor_c_cl = {
    .cpu_funcs = {nor_cipher},
    .cpu_funcs_name = {"nor_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};





void ornot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | ! *B;
#ifdef dump_mode
    std::cout << "ORNOT GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void ornot_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomORYN<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "ORNOT GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet ornot_p_cl = {
	.cpu_funcs = {ornot_plain},
    .cpu_funcs_name = {"ornot_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet ornot_c_cl = {
    .cpu_funcs = {ornot_cipher},
    .cpu_funcs_name = {"ornot_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};


void xor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A ^ *B;

#ifdef dump_mode
    std::cout << "XOR GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void xor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomXOR<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "XOR GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet xor_p_cl = {
	.cpu_funcs = {xor_plain},
    .cpu_funcs_name = {"xor_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet xor_c_cl = {
    .cpu_funcs = {xor_cipher},
    .cpu_funcs_name = {"xor_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};



void xnor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A ^ *B);
#ifdef dump_mode
    std::cout << "XNOR GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void xnor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomXNOR<lvl_param>(*Y, *A, *B, ek);
#ifdef dump_mode
    std::cout << "XNOR GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet xnor_p_cl = {
	.cpu_funcs = {xor_plain},
    .cpu_funcs_name = {"xnor_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_p_perf_model,
};

struct starpu_codelet xnor_c_cl = {
    .cpu_funcs = {xor_cipher},
    .cpu_funcs_name = {"xnor_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &bnode_c_perf_model,
};



void not_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) = !*A;
#ifdef dump_mode
    std::cout << "NOT GATE: " << (*A?"true ":"false ")  << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) ? "true" : "false") << std::endl;
#endif
}

void not_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::HomNOT<lvl_param>(*Y, *A);
#ifdef dump_mode
    std::cout << "NOT GATE rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet not_p_cl = {
	.cpu_funcs = {not_plain},
    .cpu_funcs_name = {"not_plain"},
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_RW},
    .model = &unode_p_perf_model,
};

struct starpu_codelet not_c_cl = {
    .cpu_funcs = {not_cipher},
    .cpu_funcs_name = {"not_cipher"},
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_RW},
    .model = &unode_c_perf_model,
};



void dff_p_plain(void *buffers[], void *cl_arg){
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
	if((*CLK == true) && (state[1] == false)){
		state[0] = *D;
	}
	state[1] = *CLK;
#ifdef dump_mode
    std::cout << "DFF_P" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}

void dff_p_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> edge, next;
    TFHEpp::HomANDNY<lvl_param>(edge, state[1], *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, state[0], ek);
    TFHEpp::HomCOPY<lvl_param>(*state, next);
    TFHEpp::HomCOPY<lvl_param>(state[1], *CLK);
#ifdef dump_mode
    std::cout << "DFF_P rank:"  << my_rank << std::endl;
#endif
}



struct starpu_codelet dff_p_p_cl = {
	.cpu_funcs = {dff_p_plain},
    .cpu_funcs_name = {"dff_p_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &dff_p_perf_model,
};

struct starpu_codelet dff_p_c_cl = {
    .cpu_funcs = {dff_p_cipher},
    .cpu_funcs_name = {"dff_p_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &dff_c_perf_model,
};



void dff_n_plain(void *buffers[], void *cl_arg){
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
	if((*CLK == false) && (state[1] == true)){
		state[0] = *D;
	}
	state[1] = *CLK;
#ifdef dump_mode
    std::cout << "DFF_Q" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void dff_n_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> edge, next;
    TFHEpp::HomANDYN<lvl_param>(edge, state[1], *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, state[0], ek);
    TFHEpp::HomCOPY<lvl_param>(state[0], next);
    TFHEpp::HomCOPY<lvl_param>(state[1], *CLK);
#ifdef dump_mode
    std::cout << "DFF_N rank:"  << my_rank << std::endl;
#endif
}

struct starpu_codelet dff_n_p_cl = {
	.cpu_funcs = {dff_n_plain},
    .cpu_funcs_name = {"dff_n_plain"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &dff_p_perf_model,
};

struct starpu_codelet dff_n_c_cl = {
    .cpu_funcs = {dff_n_cipher},
    .cpu_funcs_name = {"dff_n_cipher"},
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW},
    .model = &dff_c_perf_model,
};


void adff_pp1_plain(void *buffers[], void *cl_arg){
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if((*CLK == true) && (state[1] == false)){
		state[0] = *D;
	}
	state[1] = *CLK;
	if((*RST == true) && (state[2] == false)){
		*state = true;
	}
	state[2] = *RST;
#ifdef dump_mode
    std::cout << "ADFF_PP1" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN RST:" << (*RST?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}

void adff_pp1_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *RST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> cedge, redge, cnext, rnext, homtrue;
    TFHEpp::HomANDNY<lvl_param>(cedge, state[1], *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(cnext, cedge, *D, state[0], ek);
    TFHEpp::HomANDNY<lvl_param>(redge, state[2], *RST, ek);
    TFHEpp::HomCONSTANTONE<lvl_param>(homtrue);
    TFHEpp::HomMUX<lvl_param>(rnext, redge, homtrue, cnext, ek);
    TFHEpp::HomCOPY<lvl_param>(state[0], rnext);
    TFHEpp::HomCOPY<lvl_param>(state[1], *CLK);
    TFHEpp::HomCOPY<lvl_param>(state[2], *RST);
#ifdef dump_mode
    std::cout << "ADFF_PP1 rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet adff_pp1_p_cl = {
	.cpu_funcs = {adff_pp1_plain},
    .cpu_funcs_name = {"adff_pp1_plain"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &adff_p_perf_model,
};

struct starpu_codelet adff_pp1_c_cl = {
	.cpu_funcs = {adff_pp1_cipher},
    .cpu_funcs_name = {"adff_pp1_cipher"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &adff_c_perf_model,
};





void adff_pp0_plain(void *buffers[], void *cl_arg){
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if((*CLK == true) && (state[1] == false)){
		*state = *D;
	}
	state[1] = *CLK;
	if((*RST == true) && (state[2] == false)){
		*state = false;
	}
	state[2] = *RST;
#ifdef dump_mode
    std::cout << "ADFF_PP0" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN RST:" << (*RST?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void adff_pp0_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *RST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> cedge, redge, cnext, rnext, homfalse;
    TFHEpp::HomANDNY<lvl_param>(cedge, state[1], *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(cnext, cedge, *D, state[0], ek);
    TFHEpp::HomANDNY<lvl_param>(redge, state[2], *RST, ek);
    TFHEpp::HomCONSTANTZERO<lvl_param>(homfalse);
    TFHEpp::HomMUX<lvl_param>(rnext, redge, homfalse, cnext, ek);
    TFHEpp::HomCOPY<lvl_param>(state[0], rnext);
    TFHEpp::HomCOPY<lvl_param>(state[1], *CLK);
    TFHEpp::HomCOPY<lvl_param>(state[2], *RST);
#ifdef dump_mode
    std::cout << "ADFF_PP0 rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet adff_pp0_p_cl = {
	.cpu_funcs = {adff_pp0_plain},
    .cpu_funcs_name = {"adff_pp0_plain"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &adff_p_perf_model,
};

struct starpu_codelet adff_pp0_c_cl = {
	.cpu_funcs = {adff_pp0_cipher},
    .cpu_funcs_name = {"adff_pp0_plain"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &adff_c_perf_model,
};


void dffe_pp_plain(void *buffers[], void *cl_arg){
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
	bool *E = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if((*CLK == true) && (state[1] == false) && (*E == true) && (state[2] == false)){
		state[0] = *D;
	}
	state[1] = *CLK;
    state[2] = *E;
#ifdef dump_mode
    std::cout << "DFFE_PP" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN E:" << (*E?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void dffe_pp_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *E = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> cedge, eedge, edge, next;
    TFHEpp::HomANDNY<lvl_param>(cedge, state[1], *CLK, ek);
    TFHEpp::HomANDNY<lvl_param>(eedge, state[2], *E, ek);
    TFHEpp::HomAND<lvl_param>(edge, cedge, eedge, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY<lvl_param>(state[0], next);
    TFHEpp::HomCOPY<lvl_param>(state[1], *CLK);
    TFHEpp::HomCOPY<lvl_param>(state[2], *E);
#ifdef dump_mode
    std::cout << "DFFE_PP rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet dffe_pp_p_cl = {
	.cpu_funcs = {dffe_pp_plain},
    .cpu_funcs_name = {"dffe_pp_plain"},
    .nbuffers = 4,
    .modes = {STARPU_RW, STARPU_R, STARPU_R, STARPU_R},
    .model = &adff_p_perf_model,
};

struct starpu_codelet dffe_pp_c_cl = {
	.cpu_funcs = {dffe_pp_cipher},
    .cpu_funcs_name = {"dffe_pp_cipher"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &adff_c_perf_model,
};



void mux_plain(void *buffers[], void *cl_arg){
    bool *S = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) = *S ? *A : *B;
#ifdef dump_mode
    std::cout << "MUX" << std::endl;
    std::cout << "IN S:" << (*S?"true":"false") << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) ? "true" : "false") << std::endl;
#endif
}


void mux_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *S = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::HomMUX<lvl_param>(*Y, *S, *A, *B, ek);
#ifdef dump_mode
    std::cout << "MUX rank:"  << my_rank << std::endl;
#endif
}


struct starpu_codelet mux_p_cl = {
	.cpu_funcs = {mux_plain},
    .cpu_funcs_name = {"mux_plain"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &mux_p_perf_model,
};

struct starpu_codelet mux_c_cl = {
	.cpu_funcs = {mux_cipher},
    .cpu_funcs_name = {"mux_cipher"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &mux_c_perf_model,
};



inline void nmux_plain(void *buffers[], void *cl_arg){
    bool *S = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) = !*S ? *A : *B;
#ifdef dump_mode
    std::cout << "NMUX" << std::endl;
    std::cout << "IN S:" << (*S?"true":"false") << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) ? "true" : "false") << std::endl;
#endif
}

void nmux_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *S = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::HomNMUX<lvl_param>(*Y, *S, *A, *B, ek);
#ifdef dump_mode
    std::cout << "NMUX rank:"  << my_rank << std::endl;
#endif
}

struct starpu_codelet nmux_p_cl = {
	.cpu_funcs = {nmux_plain},
    .cpu_funcs_name = {"nmux_plain"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &mux_p_perf_model,
};

struct starpu_codelet nmux_c_cl = {
	.cpu_funcs = {nmux_cipher},
    .cpu_funcs_name = {"nmux_cipher"},
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW},
    .model = &mux_c_perf_model,
};


static struct starpu_perfmodel init_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "init_p_perf_model"
};

static struct starpu_perfmodel init_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "init_c_perf_model"
};



void init_plain(void *buffers[], void *cl_arg){
    bool val;
    starpu_codelet_unpack_args(cl_arg, &val);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[0]) = val;
}


void init_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> A;
    starpu_codelet_unpack_args(cl_arg, &A);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::HomCOPY<lvl_param>(*Y, A);
}

struct starpu_codelet init_p_cl = {
    .cpu_funcs = {init_plain},
    .cpu_funcs_name = {"init_plain"},
    .nbuffers = 1,
    .modes = {STARPU_W},
    .model = &init_p_perf_model,
};

struct starpu_codelet init_c_cl = {
    .cpu_funcs = {init_cipher},
    .cpu_funcs_name = {"init_cipher"},
    .nbuffers = 1,
    .modes = {STARPU_W},
    .model = &init_c_perf_model,
};

static struct starpu_perfmodel copy_p_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "copy_p_perf_model"
};

static struct starpu_perfmodel copy_c_perf_model =
{
.type = STARPU_HISTORY_BASED,
.symbol = "copy_c_perf_model"
};



void copy_plain(void *buffers[], void *cl_arg){
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) = *(bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
}


void copy_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::HomCOPY<lvl_param>(*Y, *A);
}

struct starpu_codelet copy_p_cl = {
    .cpu_funcs = {copy_plain},
    .cpu_funcs_name = {"copy_plain"},
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_W},
    .model = &copy_p_perf_model,
};

struct starpu_codelet copy_c_cl = {
    .cpu_funcs = {copy_cipher},
    .cpu_funcs_name = {"copy_cipher"},
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_W},
    .model = &copy_c_perf_model,
};
