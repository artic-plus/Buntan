#pragma once

#ifndef __nodetypes__
#define __nodetypes__


#include <string>
#include <vector>
#include <map>
#include <starpu.h>

#define plain_mode

#define bnode_inputs  {"A", "B"}
#define unode_input  {"A"}
#define dff_inputs  {"C", "D"}
#define adff_inputs  {"C", "D", "R"}
#define dffe_inputs  {"C", "D", "E"}
#define mux_inputs  {"A", "B", "S"}
#define fa_inputs  {"A", "B", "C"}
#define bnode_output  {"Y"}
#define dff_output  {"Q"}
#define fa_outputs  {"X", "Y"}

struct  nodetype
{
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	void* codelet;
	bool isFF;
};

extern void and_plain(void *buffers[], void *cl_arg);
extern void nand_plain(void *buffers[], void *cl_arg);
extern void andnot_plain(void *buffers[], void *cl_arg);
extern void or_plain(void *buffers[], void *cl_arg);
extern void nor_plain(void *buffers[], void *cl_arg);
extern void ornot_plain(void *buffers[], void *cl_arg);
extern void xor_plain(void *buffers[], void *cl_arg);
extern void xnor_plain(void *buffers[], void *cl_arg);
extern void not_plain(void *buffers[], void *cl_arg);
extern void dff_p_plain(void *buffers[], void *cl_arg);
extern void dff_n_plain(void *buffers[], void *cl_arg);
extern void adff_pp1_plain(void *buffers[], void *cl_arg);
extern void adff_pp0_plain(void *buffers[], void *cl_arg);
extern void dffe_pp_plain(void *buffers[], void *cl_arg);
extern void mux_plain(void *buffers[], void *cl_arg);
extern void nmux_plain(void *buffers[], void *cl_arg);

extern struct starpu_codelet and_cl;
extern struct starpu_codelet nand_cl;
extern struct starpu_codelet andnot_cl;
extern struct starpu_codelet or_cl;
extern struct starpu_codelet nor_cl;
extern struct starpu_codelet ornot_cl;
extern struct starpu_codelet xor_cl;
extern struct starpu_codelet xnor_cl;
extern struct starpu_codelet not_cl;
extern struct starpu_codelet dff_p_cl;
extern struct starpu_codelet dff_n_cl;
extern struct starpu_codelet adff_pp1_cl;
extern struct starpu_codelet adff_pp0_cl;
extern struct starpu_codelet dffe_pp_cl;
extern struct starpu_codelet mux_cl;
extern struct starpu_codelet nmux_cl;

std::map<std::string, nodetype*> types_init();

#endif