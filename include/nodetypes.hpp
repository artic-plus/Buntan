#pragma once

#ifndef __nodetypes__
#define __nodetypes__


#include <string>
#include <vector>
#include <map>
#include <starpu.h>
#include <tfhe++.hpp>

#define bnode_inputs  {"A", "B"}
#define unode_input  {"A"}
#define dff_inputs  {"D", "C"}
#define adff_inputs  {"D", "C", "R"}
#define dffe_inputs  {"D", "C", "E"}
#define mux_inputs  {"A", "B", "S"}
#define fa_inputs  {"A", "B", "C"}
#define bnode_output  {"Y"}
#define dff_output  {"Q"}
#define fa_outputs  {"X", "Y"}

struct nodetype{
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	std::string key;
	void* cl;
	bool isFF;
	int id;
};

extern TFHEpp::EvalKey ek;
extern int my_rank;


#ifdef BUNTAN_CUDA_ENABLED
#define use_lvl0
#endif

#ifdef use_lvl0
typedef TFHEpp::lvl0param lvl_param;
#else
typedef TFHEpp::lvl1param lvl_param;
#endif

#ifdef plain_mode
typedef bool t_val;
#else
typedef TFHEpp::TLWE<lvl_param> t_val;
#endif


extern std::map<std::string, nodetype*> nodetypes;
extern std::map<int, nodetype*> type_id;
int types_init();


void and_plain(void *buffers[], void *cl_arg);
void and_cipher(void *buffers[], void *cl_arg);
void nand_plain(void *buffers[], void *cl_arg);
void nand_cipher(void *buffers[], void *cl_arg);
void andnot_plain(void *buffers[], void *cl_arg);
void andnot_cipher(void *buffers[], void *cl_arg);
void or_plain(void *buffers[], void *cl_arg);
void or_cipher(void *buffers[], void *cl_arg);
void nor_plain(void *buffers[], void *cl_arg);
void nor_cipher(void *buffers[], void *cl_arg);
void ornot_plain(void *buffers[], void *cl_arg);
void ornot_cipher(void *buffers[], void *cl_arg);
void xor_plain(void *buffers[], void *cl_arg);
void xor_cipher(void *buffers[], void *cl_arg);
void xnor_plain(void *buffers[], void *cl_arg);
void xnor_cipher(void *buffers[], void *cl_arg);
void dff_p_plain(void *buffers[], void *cl_arg);
void dff_p_cipher(void *buffers[], void *cl_arg);
void dff_n_plain(void *buffers[], void *cl_arg);
void dff_n_cipher(void *buffers[], void *cl_arg);
void adff_po1_plain(void *buffers[], void *cl_arg);
void adff_po1_cipher(void *buffers[], void *cl_arg);
void adff_pp0_plain(void *buffers[], void *cl_arg);
void adff_pp0_cipher(void *buffers[], void *cl_arg);
void dffe_pp_plain(void *buffers[], void *cl_arg);
void dffe_pp_cipher(void *buffers[], void *cl_arg);
void mux_plain(void *buffers[], void *cl_arg);
void mux_cipher(void *buffers[], void *cl_arg);
void nmux_plain(void *buffers[], void *cl_arg);
void nmux_cipher(void *buffers[], void *cl_arg);

#endif