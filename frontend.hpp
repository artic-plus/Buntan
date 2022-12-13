#pragma once

#ifndef __frontend__
#define __frontend__

#include <queue>
#include <map>

#define PICOJSON_USE_INT64

#include "picojson.h"
#include "nodetypes.hpp"

struct  node;


struct wire{
	node* src; 
	std::queue<node*>* dep; 
	bool merged;
};

struct node{
	nodetype* type;
	void* dff_mem;
    int d_counter; //temporary counter to follow dependencies
	int num_inputs;
	std::pair<void*,wire*>* inputs; //array of std::pair //  pointer to data handle and input wire
	int num_outputs;
	wire** outputs;
};

int yosys_json_parser(std::string json_path, std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>>* inputs, std::map<std::string, std::pair<int, wire**>>* outputs, std::map<std::string, node*>* FFs, wire* ImmTrue, wire* ImmFalse);
int parsemodule(picojson::object modules, std::string mname, std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>>* inputs, std::map<std::string, std::pair<int, wire**>>* outputs, std::map<std::string, node*>* FFs, wire* ImmTrue, wire* ImmFalse);
int shownodes(std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>> inputs, std::map<std::string, std::pair<int, wire**>> outputs, std::map<std::string, node*> FFs, wire* ImmTrue, wire* ImmFalse);

#endif