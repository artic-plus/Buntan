#pragma once

#include <starpu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <map>

#include "frontend.hpp"
#include "nodetypes.hpp"


int init_FFs(std::map<std::string, std::pair<node*, uintptr_t>>* FFs);
std::vector<bool> make_inputs_plain_rand(std::map<std::string, int> inputs);
std::vector<bool> make_inputs_plain_manual(std::map<std::string, int> inputs);
std::vector<bool> deploygates_plain(std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>> inputs, std::vector<bool> arg_in, std::map<std::string, std::pair<int, wire**>> outputs, std::map<std::string, std::pair<node*, uintptr_t>> FFs, wire* ImmTrue, wire* ImmFalse);
int deploygates_cipher(std::map<std::string, nodetype*> nodetypes, std::map<std::string, std::pair<int, wire**>> inputs, std::vector<bool> arg_in, std::map<std::string, std::pair<int, wire**>> outputs, std::map<std::string, std::pair<node*, uintptr_t>> FFs, wire* ImmTrue, wire* ImmFalse);
int result_dump(std::map<std::string, int> outputs, std::vector<bool> retvals);