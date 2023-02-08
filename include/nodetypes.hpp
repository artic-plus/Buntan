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



typedef TFHEpp::TLWE<lvl_param> bool_enc;

typedef TFHEpp::SecretKey seckey;
typedef TFHEpp::EvalKey evalkey;

std::unique_ptr<seckey> gen_key(evalkey &ek);

void c_one(bool_enc &one);
void c_zero(bool_enc &zero);

std::vector<bool_enc> enc_vec(std::vector<uint8_t> plain_vec, seckey sk);
std::vector<uint8_t> dec_vec(std::vector<bool_enc> cipher_vec, seckey sk);



extern std::map<std::string, nodetype*> nodetypes;
extern std::map<int, nodetype*> type_id;
int types_init(bool plain);


extern "C"  void and_plain(void *buffers[], void *cl_arg);
extern "C"  void and_cipher(void *buffers[], void *cl_arg);
extern "C"  void nand_plain(void *buffers[], void *cl_arg);
extern "C"  void nand_cipher(void *buffers[], void *cl_arg);
extern "C"  void andnot_plain(void *buffers[], void *cl_arg);
extern "C"  void andnot_cipher(void *buffers[], void *cl_arg);
extern "C"  void or_plain(void *buffers[], void *cl_arg);
extern "C"  void or_cipher(void *buffers[], void *cl_arg);
extern "C"  void nor_plain(void *buffers[], void *cl_arg);
extern "C"  void nor_cipher(void *buffers[], void *cl_arg);
extern "C"  void ornot_plain(void *buffers[], void *cl_arg);
extern "C"  void ornot_cipher(void *buffers[], void *cl_arg);
extern "C"  void xor_plain(void *buffers[], void *cl_arg);
extern "C"  void xor_cipher(void *buffers[], void *cl_arg);
extern "C"  void xnor_plain(void *buffers[], void *cl_arg);
extern "C"  void xnor_cipher(void *buffers[], void *cl_arg);
extern "C"  void not_plain(void *buffers[], void *cl_arg);
extern "C"  void not_cipher(void *buffers[], void *cl_arg);
extern "C"  void dff_p_plain(void *buffers[], void *cl_arg);
extern "C"  void dff_p_cipher(void *buffers[], void *cl_arg);
extern "C"  void dff_n_plain(void *buffers[], void *cl_arg);
extern "C"  void dff_n_cipher(void *buffers[], void *cl_arg);
extern "C"  void adff_po1_plain(void *buffers[], void *cl_arg);
extern "C"  void adff_po1_cipher(void *buffers[], void *cl_arg);
extern "C"  void adff_pp0_plain(void *buffers[], void *cl_arg);
extern "C"  void adff_pp0_cipher(void *buffers[], void *cl_arg);
extern "C"  void dffe_pp_plain(void *buffers[], void *cl_arg);
extern "C"  void dffe_pp_cipher(void *buffers[], void *cl_arg);
extern "C"  void mux_plain(void *buffers[], void *cl_arg);
extern "C"  void mux_cipher(void *buffers[], void *cl_arg);
extern "C"  void nmux_plain(void *buffers[], void *cl_arg);
extern "C"  void nmux_cipher(void *buffers[], void *cl_arg);

extern struct starpu_codelet init_p_cl;
extern struct starpu_codelet init_c_cl;
extern struct starpu_codelet copy_p_cl;
extern struct starpu_codelet copy_c_cl;


extern "C"  void init_plain(void *buffers[], void *cl_arg);
extern "C"  void init_cipher(void *buffers[], void *cl_arg);
extern "C"  void copy_plain(void *buffers[], void *cl_arg);
extern "C"  void copy_cipher(void *buffers[], void *cl_arg);

#endif
