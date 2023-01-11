
#include <string>
#include <vector>
#include <map>
#include <starpu.h>
#include <starpu_mpi.h>

#include "nodetypes.hpp"
#include "frontend.hpp"
#include "backend.hpp"
#include <tfhe++.hpp>



extern void insert_and(node gate, starpu_data_handle_t* handle_out);
extern void insert_nand(node gate, starpu_data_handle_t* handle_out);
extern void insert_andnot(node gate, starpu_data_handle_t* handle_out);
extern void insert_or(node gate, starpu_data_handle_t* handle_out);
extern void insert_nor(node gate, starpu_data_handle_t* handle_out);
extern void insert_ornot(node gate, starpu_data_handle_t* handle_out);
extern void insert_xor(node gate, starpu_data_handle_t* handle_out);
extern void insert_xnor(node gate, starpu_data_handle_t* handle_out);
extern void insert_not(node gate, starpu_data_handle_t* handle_out);
extern void insert_dff_p(node gate, starpu_data_handle_t* handle_out);
extern void insert_dff_n(node gate, starpu_data_handle_t* handle_out);
extern void insert_adff_pp1(node gate, starpu_data_handle_t* handle_out);
extern void insert_adff_pp0(node gate, starpu_data_handle_t* handle_out);
extern void insert_dffe_pp(node gate, starpu_data_handle_t* handle_out);
extern void insert_mux(node gate, starpu_data_handle_t* handle_out);
extern void insert_nmux(node gate, starpu_data_handle_t* handle_out);

extern void insert_and_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_nand_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_andnot_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_or_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_nor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_ornot_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_xor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_xnor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_not_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_dff_p_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_dff_n_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_adff_pp1_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_adff_pp0_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_dffe_pp_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_mux_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);
extern void insert_nmux_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank);


int types_init(){
	nodetype* And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_AND_", (void*)&insert_and, (void*)&insert_and_mpi, false, 0};
	nodetype* Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NAND_", (void*)&insert_nand, (void*)&insert_nand_mpi, false, 1};
	nodetype* AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output),"$_ANDNOT_" , (void*)&insert_andnot, (void*)&insert_andnot_mpi, false, 2};
	nodetype* Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_OR_",  (void*)&insert_or, (void*)&insert_or_mpi, false, 3};
	nodetype* Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NOR_", (void*)&insert_nor, (void*)&insert_nor_mpi, false, 4};
	nodetype* OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_ORNOT_", (void*)&insert_ornot, (void*)&insert_ornot_mpi, false, 5};
	nodetype* Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XOR_", (void*)&insert_xor, (void*)&insert_xor_mpi, false, 6};
	nodetype* Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XNOR_", (void*)&insert_xnor, (void*)&insert_xnor_mpi, false, 7};
	nodetype* Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), "$_NOT_", (void*)&insert_not, (void*)&insert_not_mpi, false, 8};
	nodetype* DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_P_", (void*)&insert_dff_p, (void*)&insert_dff_p_mpi, true, 9};
	nodetype* DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_N_", (void*)&insert_dff_n, (void*)&insert_dff_n_mpi, true, 10};
	nodetype* ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP1_", (void*)&insert_adff_pp1, (void*)&insert_adff_pp1_mpi, true, 11};
	nodetype* ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP0_", (void*)&insert_adff_pp0, (void*)&insert_adff_pp0_mpi, true, 12};
	nodetype* DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), "$_DFFE_PP_", (void*)&insert_dffe_pp, (void*)&insert_dffe_pp_mpi, true, 13};
	nodetype* MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_MUX_", (void*)&insert_mux, (void*)&insert_mux_mpi, false, 14};
	nodetype* NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_NMUX_", (void*)&insert_nmux, (void*)&insert_nmux_mpi, false, 15};




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
}

struct starpu_codelet and_cl = {
#ifdef plain_mode
	.cpu_funcs = {and_plain},
#else
    .cpu_funcs = {and_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};

void insert_and(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&and_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_and_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &and_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet nand_cl = {
#ifdef plain_mode
	.cpu_funcs = {nand_plain},
#else
    .cpu_funcs = {nand_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_nand(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&nand_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_nand_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nand_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
int i = starpu_mpi_world_rank();
}


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
}

struct starpu_codelet andnot_cl = {
#ifdef plain_mode
	.cpu_funcs = {andnot_plain},
#else
    .cpu_funcs = {andnot_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_andnot(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&andnot_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_andnot_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &andnot_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet or_cl = {
#ifdef plain_mode
	.cpu_funcs = {or_plain},
#else
    .cpu_funcs = {or_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_or(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&or_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_or_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &or_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet nor_cl = {
#ifdef plain_mode
	.cpu_funcs = {nor_plain},
#else
    .cpu_funcs = {nor_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_nor(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&nor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_nor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nor_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}


void ornot_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *A = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *B = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *Y = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomORYN<lvl_param>(*Y, *A, *B, ek);
}

void ornot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | ! *B;
#ifdef dump_mode
    std::cout << "ORNOT GATE: "  << (*A?"true ":"false ")  << (*B?"true ":"false ") << "->" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

struct starpu_codelet ornot_cl = {
#ifdef plain_mode
	.cpu_funcs = {ornot_plain},	
#else
    .cpu_funcs = {ornot_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_ornot(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&ornot_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_ornot_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &ornot_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet xor_cl = {
#ifdef plain_mode
	.cpu_funcs = {xor_plain},
#else
    .cpu_funcs = {xor_cipher},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_xor(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&xor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_xor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &xor_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet xnor_cl = {
#ifdef plain_mode
	.cpu_funcs = {xnor_plain},
#else	
    .cpu_funcs = {xnor_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_RW}
};


void insert_xnor(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&xnor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_xnor_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &xnor_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        0);
}

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
}

struct starpu_codelet not_cl = {
#ifdef plain_mode
	.cpu_funcs = {not_plain},
#else
    .cpu_funcs = {not_cipher},
#endif
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_RW}
};


void insert_not(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&not_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_RW, *handle_output,  
        0);
}

void insert_not_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &not_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        0);
}



void dff_p_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if((*CLK == true) && (*fCLK == false)){
		*state = *D;
	}
	*fCLK = *CLK;
#ifdef dump_mode
    std::cout << "DFF_P" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}

void dff_p_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *fCLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> edge, next;
    TFHEpp::HomANDNY<lvl_param>(edge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY<lvl_param>(*state, next);
    TFHEpp::HomCOPY<lvl_param>(*fCLK, *CLK);
}

struct starpu_codelet dff_p_cl = {
#ifdef plain_mode
	.cpu_funcs = {dff_p_plain},
#else
    .cpu_funcs = {dff_p_cipher},
#endif
    .nbuffers = 4,
    .modes = {STARPU_RW, STARPU_RW, STARPU_R, STARPU_R}
};


void insert_dff_p(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&dff_p_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        0);
}

void insert_dff_p_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dff_p_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_RW, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        STARPU_R, wire_handles[wire_id[3]],
        STARPU_R, wire_handles[wire_id[4]],
        0);
}

void dff_n_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if((*CLK == false) && (*fCLK == true)){
		*state = *D;
	}
	*fCLK = *CLK;
#ifdef dump_mode
    std::cout << "DFF_Q" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void dff_n_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *fCLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> edge, next;
    TFHEpp::HomANDYN<lvl_param>(edge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY<lvl_param>(*state, next);
    TFHEpp::HomCOPY<lvl_param>(*fCLK, *CLK);
}


struct starpu_codelet dff_n_cl = {
#ifdef plain_mode
	.cpu_funcs = {dff_n_plain},
#else
    .cpu_funcs = {dff_n_cipher},
#endif
    .nbuffers = 4,
    .modes = {STARPU_RW, STARPU_RW, STARPU_R, STARPU_R}
};



void insert_dff_n(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&dff_n_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        0);
}


void insert_dff_n_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dff_n_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_RW, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        STARPU_R, wire_handles[wire_id[3]],
        STARPU_R, wire_handles[wire_id[4]],
        0);
}

void adff_pp1_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
	bool *fRST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	if((*CLK == true) && (*fCLK == false)){
		*state = *D;
	}
	*fCLK = *CLK;
	if((*RST == true) && (*fRST == false)){
		*state = true;
	}
	*fRST = *RST;
#ifdef dump_mode
    std::cout << "ADFF_PP1" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN RST:" << (*RST?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}

void adff_pp1_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *fCLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *fRST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<lvl_param> *RST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<lvl_param> cedge, redge, cnext, rnext, homtrue;
    TFHEpp::HomANDNY<lvl_param>(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(cnext, cedge, *D, *state, ek);
    TFHEpp::HomANDNY<lvl_param>(redge, *fRST, *RST, ek);
    TFHEpp::HomCONSTANTONE<lvl_param>(homtrue);
    TFHEpp::HomMUX<lvl_param>(rnext, redge, homtrue, cnext, ek);
    TFHEpp::HomCOPY<lvl_param>(*state, rnext);
    TFHEpp::HomCOPY<lvl_param>(*fCLK, *CLK);
    TFHEpp::HomCOPY<lvl_param>(*fRST, *RST);
}


struct starpu_codelet adff_pp1_cl = {
#ifdef plain_mode
	.cpu_funcs = {adff_pp1_plain},
#else
	.cpu_funcs = {adff_pp1_cipher},
#endif
    .nbuffers = 6,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R}
};



void insert_adff_pp1(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&adff_pp1_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[2]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        0);
}

void insert_adff_pp1_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &adff_pp1_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_RW, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        STARPU_R, wire_handles[wire_id[3]],
        STARPU_R, wire_handles[wire_id[4]],
        STARPU_R, wire_handles[wire_id[5]],
        0);
}

void adff_pp0_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
	bool *fRST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	if((*CLK == true) && (*fCLK == false)){
		*state = *D;
	}
	*fCLK = *CLK;
	if((*RST == true) && (*fRST == false)){
		*state = false;
	}
	*fRST = *RST;
#ifdef dump_mode
    std::cout << "ADFF_PP0" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN RST:" << (*RST?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void adff_pp0_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *fCLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *fRST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<lvl_param> *RST = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<lvl_param> cedge, redge, cnext, rnext, homfalse;
    TFHEpp::HomANDNY<lvl_param>(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<lvl_param>(cnext, cedge, *D, *state, ek);
    TFHEpp::HomANDNY<lvl_param>(redge, *fRST, *RST, ek);
    TFHEpp::HomCONSTANTZERO<lvl_param>(homfalse);
    TFHEpp::HomMUX<lvl_param>(rnext, redge, homfalse, cnext, ek);
    TFHEpp::HomCOPY<lvl_param>(*state, rnext);
    TFHEpp::HomCOPY<lvl_param>(*fCLK, *CLK);
    TFHEpp::HomCOPY<lvl_param>(*fRST, *RST);
}

struct starpu_codelet adff_pp0_cl = {
#ifdef plain_mode
	.cpu_funcs = {adff_pp0_plain},
#else
	.cpu_funcs = {adff_pp1_cipher},
#endif
    .nbuffers = 6,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R}
};

void insert_adff_pp0(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&adff_pp0_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[2]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        0);
}


void insert_adff_pp0_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &adff_pp0_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_RW, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        STARPU_R, wire_handles[wire_id[3]],
        STARPU_R, wire_handles[wire_id[4]],
        STARPU_R, wire_handles[wire_id[5]],
        0);
}

void dffe_pp_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
    bool *fE = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
	bool *E = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	if((*CLK == true) && (*fCLK == false) && (*E == true) && (*fE == false)){
		*state = *D;
	}
	*fCLK = *CLK;
    *fE = *E;
#ifdef dump_mode
    std::cout << "DFFE_PP" << std::endl;
    std::cout << "IN D:" << (*D?"true":"false") << std::endl;
    std::cout << "IN CLK:" << (*CLK?"true":"false") << std::endl;
    std::cout << "IN E:" << (*E?"true":"false") << std::endl;
    std::cout << "Q :" << (*state ? "true" : "false") << std::endl;
#endif
}


void dffe_pp_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<lvl_param> *state = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<lvl_param> *fCLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<lvl_param> *fE = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<lvl_param> *D = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<lvl_param> *CLK = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<lvl_param> *E = (TFHEpp::TLWE<lvl_param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<lvl_param> cedge, eedge, edge, next;
    TFHEpp::HomANDNY<lvl_param>(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomANDNY<lvl_param>(eedge, *fE, *E, ek);
    TFHEpp::HomAND<lvl_param>(edge, cedge, eedge, ek);
    TFHEpp::HomMUX<lvl_param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY<lvl_param>(*state, next);
    TFHEpp::HomCOPY<lvl_param>(*fCLK, *CLK);
    TFHEpp::HomCOPY<lvl_param>(*fE, *E);
}

struct starpu_codelet dffe_pp_cl = {
#ifdef plain_mode
	.cpu_funcs = {dffe_pp_plain},
#else
	.cpu_funcs = {dffe_pp_cipher},
#endif
    .nbuffers = 6,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R}
};


void insert_dffe_pp(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&dffe_pp_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[3]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        0);
}


void insert_dffe_pp_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dffe_pp_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_RW, wire_handles[wire_id[0]],
        STARPU_RW, wire_handles[wire_id[1]],
        STARPU_RW, wire_handles[wire_id[2]],
        STARPU_R, wire_handles[wire_id[3]],
        STARPU_R, wire_handles[wire_id[4]],
        STARPU_R, wire_handles[wire_id[5]],
        0);
}


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
}

struct starpu_codelet mux_cl = {
#ifdef plain_mode
	.cpu_funcs = {mux_plain},
#else
	.cpu_funcs = {mux_cipher},
#endif
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW}
};

void insert_mux(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&mux_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_mux_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &mux_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_R, wire_handles[wire_id[2]],
        STARPU_RW, wire_handles[wire_id[3]],
        0);
}


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
}

struct starpu_codelet nmux_cl = {
#ifdef plain_mode
	.cpu_funcs = {nmux_plain},
#else
	.cpu_funcs = {nmux_cipher},
#endif
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_RW}
};

void insert_nmux(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&nmux_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_nmux_mpi(starpu_data_handle_t* wire_handles, int* wire_id, int rank){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nmux_cl,
        STARPU_EXECUTE_ON_NODE, rank,
        STARPU_R, wire_handles[wire_id[0]],
        STARPU_R, wire_handles[wire_id[1]],
        STARPU_R, wire_handles[wire_id[2]],
        STARPU_RW, wire_handles[wire_id[3]],
        0);
}






