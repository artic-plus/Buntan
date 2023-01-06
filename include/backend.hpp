#pragma once

#include <starpu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <map>

#include <tfhe++.hpp>

#include "frontend.hpp"
#include "nodetypes.hpp"

extern TFHEpp::EvalKey ek;

extern int num_workers;

int init_FFs(std::map<std::string, std::pair<node*, t_val*>>* FFs);
std::vector<t_val> make_inputs_rand(std::map<std::string, int> inputs
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
);
std::vector<t_val> make_inputs_manual(std::map<std::string, int> inputs
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
);
std::vector<t_val> deploygates(std::map<std::string, std::pair<int, wire**>> inputs, std::vector<t_val> arg_in, std::map<std::string, std::pair<int, wire**>> outputs, std::map<std::string, std::pair<node*, t_val*>> FFs, wire* ImmTrue, wire* ImmFalse);
std::vector<std::pair<node, starpu_data_handle_t*>> reg_handles_mpi(std::map<std::string, std::pair<int, wire**>> inputs, std::vector<t_val> arg_in, std::map<std::string, std::pair<int, wire**>> outputs, std::vector<starpu_data_handle_t>* retval_handles, std::map<std::string, std::pair<node*, t_val*>> FFs, wire* ImmTrue, wire* ImmFalse);

int save_result(std::vector<starpu_data_handle_t> out_handles, std::string file_name);

int result_dump(std::map<std::string, int> outputs, std::vector<t_val> retvals
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
);
