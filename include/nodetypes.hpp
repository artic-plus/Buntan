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
#define dff_inputs  {"C", "D"}
#define adff_inputs  {"C", "D", "R"}
#define dffe_inputs  {"C", "D", "E"}
#define mux_inputs  {"A", "B", "S"}
#define fa_inputs  {"A", "B", "C"}
#define bnode_output  {"Y"}
#define dff_output  {"Q"}
#define fa_outputs  {"X", "Y"}

struct nodetype{
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	void* task_insert;
	bool isFF;
};

#ifdef plain_mode
typedef bool t_val;
#else
typedef TFHEpp::TLWE<TFHEpp::lvl1param> t_val;
#endif


extern std::map<std::string, nodetype*> nodetypes;
int types_init();

#endif