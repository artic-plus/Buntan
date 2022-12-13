#pragma once

#ifndef __nodetypes__
#define __nodetypes__


#include <string>
#include <vector>
#include <map>


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

extern void* and_cl;
extern void* nand_cl;
extern void* andnot_cl;
extern void* or_cl;
extern void* nor_cl;
extern void* ornot_cl;
extern void* xor_cl;
extern void* xnor_cl;
extern void* not_cl;
extern void* dff_p_cl;
extern void* dff_n_cl;
extern void* adff_pp1_cl;
extern void* adff_pp0_cl;
extern void* dffe_pp_cl;
extern void* mux_cl;
extern void* nmux_cl;
extern void* fa_cl;


inline std::map<std::string, nodetype*> types_init(){
	nodetype* And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), and_cl, false};
	nodetype* Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), nand_cl, false};
	nodetype* AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), andnot_cl, false};
	nodetype* Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), or_cl, false};
	nodetype* Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), nor_cl, false};
	nodetype* OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), ornot_cl, false};
	nodetype* Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), xor_cl, false};
	nodetype* Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), xnor_cl, false};
	nodetype* Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), not_cl, false};
	nodetype* DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), dff_p_cl, true};
	nodetype* DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), dff_n_cl, true};
	nodetype* ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), adff_pp1_cl, true};
	nodetype* ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), adff_pp1_cl, true};
	nodetype* DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), dffe_pp_cl, true};
	nodetype* MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), mux_cl, false};
	nodetype* NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), nmux_cl, false};
	nodetype* FA = new nodetype{std::vector<std::string>(fa_inputs), std::vector<std::string>(fa_outputs), fa_cl, false}; // not used




	std::map<std::string, nodetype*> nodetypes{
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
		{"$_NMUX_", NMUX},
		{"$fa", FA}
	};
	return nodetypes;
}

#endif