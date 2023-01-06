
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
extern void simple_dff(node gate, starpu_data_handle_t* handle_out);
extern void simple_dffe(node gate, starpu_data_handle_t* handle_out);



int types_init(){
	nodetype* And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_AND_", (void*)&insert_and, (void*)&insert_and_mpi, false};
	nodetype* Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NAND_", (void*)&insert_nand, (void*)&insert_nand_mpi, false};
	nodetype* AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output),"$_ANDNOT_" , (void*)&insert_andnot, (void*)&insert_andnot_mpi, false};
	nodetype* Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_OR_",  (void*)&insert_or, (void*)&insert_or_mpi, false};
	nodetype* Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_NOR_", (void*)&insert_nor, (void*)&insert_nor_mpi, false};
	nodetype* OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_ORNOT_", (void*)&insert_ornot, (void*)&insert_ornot_mpi, false};
	nodetype* Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XOR_", (void*)&insert_xor, (void*)&insert_xor_mpi, false};
	nodetype* Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), "$_XNOR_", (void*)&insert_xnor, (void*)&insert_xnor_mpi, false};
	nodetype* Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), "$_NOT_", (void*)&insert_not, (void*)&insert_not_mpi, false};
	nodetype* DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_P_", (void*)&insert_dff_p, (void*)&insert_dff_p_mpi, true};
	nodetype* DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), "$_DFF_N_", (void*)&insert_dff_n, (void*)&insert_dff_n_mpi, true};
	nodetype* ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP1_", (void*)&insert_adff_pp1, (void*)&insert_adff_pp1_mpi, true};
	nodetype* ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), "$_DFF_PP0_", (void*)&insert_adff_pp0, (void*)&insert_adff_pp0_mpi, true};
	nodetype* DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), "$_DFFE_PP_", (void*)&insert_dffe_pp, (void*)&insert_dffe_pp_mpi, true};
	nodetype* MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_MUX_", (void*)&insert_mux, (void*)&insert_mux_mpi, false};
	nodetype* NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), "$_NMUX_", (void*)&insert_nmux, (void*)&insert_nmux_mpi, false};




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
	return 0;
}

void and_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & *B;
#ifdef dump_mode
    std::cout << "AND GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void and_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomAND<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_and_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &and_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void nand_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !( *A & *B);
#ifdef dump_mode
    std::cout << "NAND GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}


void nand_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomNAND<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_nand_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nand_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void andnot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & !*B;
#ifdef dump_mode
    std::cout << "ANDNOT GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}


void andnot_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomANDYN<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_andnot_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &andnot_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void or_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | *B;

#ifdef dump_mode
    std::cout << "OR GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void or_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomOR<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_or_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &or_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void nor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A | *B);
#ifdef dump_mode
    std::cout << "NOR GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void nor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomNOR<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_nor_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}


void ornot_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomORYN<TFHEpp::lvl1param>(*Y, *A, *B, ek);
}

void ornot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | ! *B;
#ifdef dump_mode
    std::cout << "ORNOT GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

struct starpu_codelet ornot_cl = {
#ifdef plain_mode
	.cpu_funcs = {ornot_plain},	
#else
    .cpu_funcs = {ornot_cipher},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};


void insert_ornot(node gate, starpu_data_handle_t* handle_output){
	starpu_task_insert(&ornot_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void insert_ornot_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &ornot_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void xor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A ^ *B;

#ifdef dump_mode
    std::cout << "XOR GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void xor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomXOR<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_xor_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &xor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void xnor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A ^ *B);
#ifdef dump_mode
    std::cout << "XNOR GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "IN B:" << (*B?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) ? "true" : "false") << std::endl;
#endif
}

void xnor_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::HomXNOR<TFHEpp::lvl1param>(*Y, *A, *B, ek);
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

void insert_xnor_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &xnor_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_RW, *handle_output,  
        0);
}

void not_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) = !*A;
#ifdef dump_mode
    std::cout << "NOT GATE" << std::endl;
    std::cout << "IN A:" << (*A?"true":"false") << std::endl;
    std::cout << "OUT :" << (*(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) ? "true" : "false") << std::endl;
#endif
}

void not_cipher(void *buffers[], void *cl_arg){
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::HomNOT<TFHEpp::lvl1param>(*Y, *A);
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

void insert_not_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &not_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_RW, *handle_output,  
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *state = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fCLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *D = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *CLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<TFHEpp::lvl1param> edge, next;
    TFHEpp::HomANDNY(edge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY(*state, next);
    TFHEpp::HomCOPY(*fCLK, *CLK);
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

void insert_dff_p_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dff_p_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *state = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fCLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *D = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *CLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<TFHEpp::lvl1param> edge, next;
    TFHEpp::HomANDYN(edge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY(*state, next);
    TFHEpp::HomCOPY(*fCLK, *CLK);
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


void insert_dff_n_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dff_n_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *state = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fCLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fRST = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *D = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *CLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *RST = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<TFHEpp::lvl1param> cedge, redge, cnext, rnext, homtrue;
    TFHEpp::HomANDNY(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(cnext, cedge, *D, *state, ek);
    TFHEpp::HomANDNY(redge, *fRST, *RST, ek);
    TFHEpp::HomCONSTANTONE(homtrue);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(rnext, redge, homtrue, cnext, ek);
    TFHEpp::HomCOPY(*state, rnext);
    TFHEpp::HomCOPY(*fCLK, *CLK);
    TFHEpp::HomCOPY(*fRST, *RST);
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

void insert_adff_pp1_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &adff_pp1_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[2]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *state = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fCLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fRST = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *D = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *CLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *RST = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<TFHEpp::lvl1param> cedge, redge, cnext, rnext, homfalse;
    TFHEpp::HomANDNY(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(cnext, cedge, *D, *state, ek);
    TFHEpp::HomANDNY(redge, *fRST, *RST, ek);
    TFHEpp::HomCONSTANTZERO(homfalse);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(rnext, redge, homfalse, cnext, ek);
    TFHEpp::HomCOPY(*state, rnext);
    TFHEpp::HomCOPY(*fCLK, *CLK);
    TFHEpp::HomCOPY(*fRST, *RST);
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


void insert_adff_pp0_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &adff_pp0_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[2]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *state = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fCLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *fE = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *D = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *CLK = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *E = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[5]);
    TFHEpp::TLWE<TFHEpp::lvl1param> cedge, eedge, edge, next;
    TFHEpp::HomANDNY(cedge, *fCLK, *CLK, ek);
    TFHEpp::HomANDNY(eedge, *fE, *E, ek);
    TFHEpp::HomAND(edge, cedge, eedge, ek);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(next, edge, *D, *state, ek);
    TFHEpp::HomCOPY(*state, next);
    TFHEpp::HomCOPY(*fCLK, *CLK);
    TFHEpp::HomCOPY(*fE, *E);
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


void insert_dffe_pp_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &dffe_pp_cl,
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[0]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[1]),
        STARPU_RW, (((starpu_data_handle_t*)gate.dff_mem)[3]),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *S = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::HomMUX<TFHEpp::lvl1param>(*Y, *S, *A, *B, ek);
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

void insert_mux_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &mux_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        STARPU_RW, *handle_output,  
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
    TFHEpp::TLWE<TFHEpp::lvl1param> *S = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *A = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *B = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    TFHEpp::TLWE<TFHEpp::lvl1param> *Y = (TFHEpp::TLWE<TFHEpp::lvl1param>*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    TFHEpp::HomNMUX<TFHEpp::lvl1param>(*Y, *S, *A, *B, ek);
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

void insert_nmux_mpi(node gate, starpu_data_handle_t* handle_output){
	starpu_mpi_task_insert(MPI_COMM_WORLD, &nmux_cl,
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[0].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[1].first),  
        STARPU_R, *((starpu_data_handle_t*)gate.inputs[2].first),
        STARPU_RW, *handle_output,  
        0);
}






