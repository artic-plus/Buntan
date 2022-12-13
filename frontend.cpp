#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <utility>
#include <queue>
#include <map>

#define PICOJSON_USE_INT64

#include "picojson.h"
#include "nodetypes.hpp"
#include "frontend.hpp"



int yosys_json_parser(
    std::string json_path, 
    std::map<std::string, nodetype*> nodetypes, 
    std::map<std::string, std::pair<int, wire**>>* inputs, 
    std::map<std::string, std::pair<int, wire**>>* outputs, 
    std::map<std::string, node*>* FFs, 
    wire* ImmTrue, 
    wire* ImmFalse
    ){

	std::ifstream ifs(json_path);
	if (ifs.fail()){
		std::cerr << "failed to read" << json_path <<std::endl;
		return 1;
	}
	
	
	const std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	
	picojson::value v;
	const std::string err = picojson::parse(v,json);
	if(!err.empty()){
		std::cerr << err << std::endl;
		return 2;
	}
	 
	
	picojson::object& obj = v.get<picojson::object>();
	
	
	//std::cout << json_path << std::endl;
	picojson::object& modules = obj["modules"].get<picojson::object>();
	std::string top;
	for(picojson::object::const_iterator it = modules.begin(); it != modules.end(); it++){
		picojson::object module = it->second.get<picojson::object>();
		if (module["attributes"].get<picojson::object>()["top"].is<std::string>()){
			if (std::stoi(module["attributes"].get<picojson::object>()["top"].get<std::string>())){
				top = it->first;
				break;
			}
		}
	}
	std::cout << "top moodule is '" << top << "'" << std::endl;

    if (parsemodule(modules, top, nodetypes, inputs, outputs, FFs, ImmTrue, ImmFalse)){
        return 1;
    }
    std::cout << std::endl; 
    //shownodes(nodetypes, *inputs, *outputs, *FFs, ImmTrue, ImmFalse);
    return 0;
}

int parsemodule(
    picojson::object modules, 
    std::string mname, 
    std::map<std::string, nodetype*> nodetypes, 
    std::map<std::string, std::pair<int, wire**>>* inputs, 
    std::map<std::string, std::pair<int, wire**>>* outputs, 
    std::map<std::string, node*>* FFs, 
    wire* ImmTrue, 
    wire* ImmFalse
){
    picojson::object module = modules[mname].get<picojson::object>();
	std::map<int, wire*> wires{}; //wire list for dependency resolution
    std::queue<std::pair<int, wire*>> eq_wires;


    //define all wires
	picojson::object netnames = module["netnames"].get<picojson::object>();
	for(picojson::object::const_iterator it = netnames.begin(); it != netnames.end(); it++){
		picojson::object netname = it->second.get<picojson::object>();
		picojson::array bits = netname["bits"].get<picojson::array>();
		for(int i = 0; i < bits.size(); i++){
            if(bits[i].is<int64_t>()){
			    wires.insert(std::make_pair(bits[i].get<int64_t>(), new wire{0, new std::queue<node*>, false}));
            }
		}
	}
    //parse each cells
	picojson::object cells = module["cells"].get<picojson::object>();
	for(picojson::object::const_iterator it = cells.begin(); it != cells.end(); it++){
		picojson::object cell = it->second.get<picojson::object>();
		picojson::object connections = cell["connections"].get<picojson::object>();
		std::string celltype = cell["type"].get<std::string>();
        auto type = nodetypes.find(celltype);
        if(type == nodetypes.end()){
            std::map<std::string, std::pair<int, wire**>> cell_inputs{};
            std::map<std::string, std::pair<int, wire**>> cell_outputs{};
            if(parsemodule(modules, celltype, nodetypes, &cell_inputs, &cell_outputs, FFs, ImmTrue, ImmFalse)){
                std::cerr << "parsemodule of '" << celltype << "' failed" << std::endl;
                return 1;
            }
            wire* io_gl;
            for(auto out_loc = cell_outputs.begin(); out_loc != cell_outputs.end(); out_loc++){
                picojson::array port = connections[out_loc->first].get<picojson::array>();
                for(int i = 0; i < out_loc->second.first; i++){
                    eq_wires.push(std::make_pair(port[i].get<int64_t>(), out_loc->second.second[i]));
                }
            }
            for(auto in_loc = cell_inputs.begin(); in_loc != cell_inputs.end(); in_loc++){
                picojson::array port = connections[in_loc->first].get<picojson::array>();
                for(int i = 0; i < in_loc->second.first; i++){
                    if(port[i].is<int64_t>()){
                        eq_wires.push(std::make_pair(port[i].get<int64_t>(), in_loc->second.second[i]));
                    } else {
                        if(stoi(port[i].get<std::string>())) {
                            io_gl = ImmTrue;                            
                        } else {
                            io_gl = ImmFalse;
                        }
                        while (!in_loc->second.second[i]->dep->empty())
                        {
                            node* dep = in_loc->second.second[i]->dep->front();
                            io_gl->dep->push(dep);
                            for(int i = 0; i < dep->num_inputs; i++){
                                if(dep->inputs[i].second == in_loc->second.second[i]){
                                    dep->inputs[i].second = io_gl;
                                    break;
                                }
                            }
                            in_loc->second.second[i]->dep->pop();
                        }
                    }
                }
            }
            std::cout << "parse of '" << it->first << "' complete!" << std::endl;
        } else {
            node* thisnode = new node;
            thisnode->type = type->second;
            thisnode->dff_mem = nullptr;
            thisnode->d_counter = type->second->inputs.size();
            thisnode->num_inputs = type->second->inputs.size();
            thisnode->inputs = (std::pair<void*,wire*>*)malloc(sizeof(std::pair<void*,wire*>) * type->second->inputs.size());
            thisnode->num_outputs = type->second->outputs.size();
            thisnode->outputs = (wire**)malloc(sizeof(wire*) * type->second->outputs.size());
            for(int i = 0; i < thisnode->num_inputs; i++){
                picojson::array inputsarr = connections[type->second->inputs[i]].get<picojson::array>();
                if(inputsarr[0].is<int64_t>()){
                    thisnode->inputs[i] = std::make_pair(nullptr, wires[inputsarr[0].get<int64_t>()]);
                    wires[inputsarr[0].get<int64_t>()]->dep->push(thisnode);
                } else if(stoi(inputsarr[0].get<std::string>())) {
                    thisnode->inputs[i] = std::make_pair(nullptr, ImmTrue);
                } else {
                    thisnode->inputs[i] = std::make_pair(nullptr, ImmFalse);
                }    
            }
            for(int i = 0; i < thisnode->num_outputs; i++){
                picojson::array outputsarr = connections[type->second->outputs[i]].get<picojson::array>();
                thisnode->outputs[i] = wires[outputsarr[0].get<int64_t>()];
                thisnode->outputs[i]->src = thisnode;
            }
            if(thisnode->type->isFF){
                FFs->insert(std::make_pair(it->first, thisnode));
            }
        }
    }
    std::queue<wire*> merged;
    while (!eq_wires.empty()) //merge equivalent wires 
    {
        wire* first = wires[eq_wires.front().first];
        wire* second = eq_wires.front().second;
        while(first->merged) first = (wire*)first->src;
        while(second->merged) second = (wire*)second->src;
        if((first == ImmTrue || first == ImmFalse)) {
            while (!second->dep->empty())
            {
                node* dep = second->dep->front();
                first->dep->push(dep);
                for(int i = 0; i < dep->num_inputs; i++){
                    if(dep->inputs[i].second == second){
                        dep->inputs[i].second = first;
                        break;
                    }
                }
                second->dep->pop();
            }
            second->merged = true;
            second->src = (node*)first;
            merged.push(second);
        } else {
            if(first->src){
                second->src = first->src;
                for(int i = 0; i < second->src->num_outputs; i++){
                    if(second->src->outputs[i] = first){
                        second->src->outputs[i] = second;
                        break;
                    }
                }
            }
            while (!first->dep->empty())
            {
                node* dep = first->dep->front();
                second->dep->push(dep);
                for(int i = 0; i < dep->num_inputs; i++){
                    if(dep->inputs[i].second == first){
                        dep->inputs[i].second = second;
                        break;
                    }
                }
                first->dep->pop();
            }
            first->merged = true;
            first->src = (node*)second;
            merged.push(first);
        }
        eq_wires.pop();
    }
	//make i-o map 
	picojson::object ports = module["ports"].get<picojson::object>();
	for(picojson::object::const_iterator it = ports.begin(); it != ports.end(); it++){
		picojson::object port = it->second.get<picojson::object>();
		std::string direction = port["direction"].get<std::string>();
        picojson::array bits = port["bits"].get<picojson::array>();
		auto io = (wire**)malloc(sizeof(wire) * bits.size());
		for(int i = 0; i < bits.size(); i++){
            if(bits[i].is<int64_t>()){
			    io[i] = wires[bits[i].get<int64_t>()];
                while(io[i]->merged) io[i] = (wire*)io[i]->src; 
            } else if(stoi(bits[i].get<std::string>())){
                io[i] = ImmTrue;
            } else {
                io[i] = ImmFalse;
            }
		}
		if(direction == "input"){
			inputs->insert(std::make_pair(it->first,std::make_pair(bits.size(),io)));
		} else if(direction == "output"){
			outputs->insert(std::make_pair(it->first,std::make_pair(bits.size(),io)));
		} else{
			std::cerr << "port direction '" << direction << "' not supported" << std::endl;
			return 1;
		}
	}
    while (!merged.empty())
    {
        delete merged.front();
        merged.pop();
    }
    return 0;
}

int shownodes(
    std::map<std::string, nodetype*> nodetypes, 
    std::map<std::string, std::pair<int, wire**>> inputs, 
    std::map<std::string, std::pair<int, wire**>> outputs, 
    std::map<std::string, node*> FFs, 
    wire* ImmTrue, 
    wire* ImmFalse
){
    std::queue<wire*> wires;
    wires.push(ImmTrue);
    wires.push(ImmFalse);
    for(auto it = inputs.begin(); it != inputs.end(); it++){
        std::cout << "input:" << it->first << " " << it->second.first << "bit" << std::endl;
        for(int i = 0; i < it->second.first; i++){
            wires.push(it->second.second[i]);
        }
    }
    std::cout << std::endl;    
    for(auto it = outputs.begin(); it != outputs.end(); it++){
        std::cout << it->first <<" " << it->second.first << "bit" << std::endl;
    }
    for(auto FF = FFs.begin(); FF != FFs.end(); FF++){
        //std::cout << "FF   :" << FF->first << " " << FF->second->num_outputs << "bit" << std::endl;
        wires.push(FF->second->outputs[0]);
    }
    
    wire* next;
    while (!wires.empty())
    {
        next = wires.front();
        for(auto it = FFs.begin(); it != FFs.end(); it++){
            if(it->second->inputs[0].second == next || it->second->inputs[1].second == next){
                //std::cout << "FF     '" << it->first << "' input obtained!" << std::endl;
            }
        }
        for(auto it = outputs.begin() ; it != outputs.end(); it++){
            for(int i = 0; i < it->second.first; i++){
                if(it->second.second[i] == next){
                    std::cout << "output '" << it->first << "[" << i <<"]' obtained!" << std::endl;
                }
            }
        }
        auto newqueue = new std::queue<node*>;
        while (!next->dep->empty())
        {
            next->dep->front()->d_counter--;
            if((next->dep->front()->d_counter == 0) && !(next->dep->front()->type->isFF)){
                for(int i = 0; i < next->dep->front()->num_outputs; i++){
                    wires.push(next->dep->front()->outputs[i]);
                }
                next->dep->front()->d_counter = next->dep->front()->num_inputs;
            } 
            newqueue->push(next->dep->front());
            next->dep->pop();
        }
        delete next->dep;
        next->dep = newqueue;
        wires.pop();
    }
    return 0;   
}