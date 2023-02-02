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


extern int world_size;
extern int num_mems;

extern struct starpu_codelet init_cl;

extern struct starpu_codelet copy_cl;

int init_FFs(std::map<std::string, std::pair<node*, t_val*>>* FFs);
std::vector<t_val> make_inputs_rand(std::map<std::string, int> inputs
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
, int n = 1
);
std::vector<t_val> make_inputs_manual(std::map<std::string, int> inputs
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
, int n = 1
);
int deploygates(std::map<std::string, std::pair<int, wire**>> inputs, t_val* arg_in, std::map<std::string, std::pair<int, wire**>> outputs, std::vector<t_val>* retvals, std::map<std::string, std::pair<node*, t_val*>> FFs, wire* ImmTrue, wire* ImmFalse);

int gate_distrib(int gate_index);

extern struct starpu_codelet copy_cl;

std::vector<int>* reg_handles(std::map<std::string, std::pair<int, wire**>> inputs, int* arg_handle_id, std::map<std::string, std::pair<int, wire**>> outputs, int* retval_handles, t_val** retval_ptrs, std::map<std::string, std::pair<node*, t_val*>> FFs, starpu_data_handle_t* wire_handles, int numwires, wire* ImmTrue, wire* ImmFalse);

std::vector<int>* reg_handles_mpi(std::map<std::string, std::pair<int, wire**>> inputs, int* arg_handle_id, std::map<std::string, std::pair<int, wire**>> outputs, int* retval_handles, t_val** retval_ptrs, std::map<std::string, std::pair<node*, t_val*>> FFs, starpu_data_handle_t* wire_handles, int numwires, wire* ImmTrue, wire* ImmFalse);

int insert_tasks(std::vector<int>* tasks, starpu_data_handle_t* wire_handles);

int insert_tasks_mpi(std::vector<int>* tasks, starpu_data_handle_t* wire_handles);

int result_dump(std::map<std::string, int> outputs, std::vector<t_val> retvals
#ifndef plain_mode
, TFHEpp::SecretKey sk
#endif
, int n = 1
);
