
#include <string>
#include <vector>
#include <map>
#include <starpu.h>

#include "nodetypes.hpp"


std::map<std::string, nodetype*> types_init(){
	nodetype* And = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&and_cl, false};
	nodetype* Nand = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&nand_cl, false};
	nodetype* AndNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&andnot_cl, false};
	nodetype* Or = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&or_cl, false};
	nodetype* Nor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&nor_cl, false};
	nodetype* OrNot = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&ornot_cl, false};
	nodetype* Xor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&xor_cl, false};
	nodetype* Xnor = new nodetype{std::vector<std::string>(bnode_inputs), std::vector<std::string>(bnode_output), (void*)&xnor_cl, false};
	nodetype* Not = new nodetype{std::vector<std::string>(unode_input), std::vector<std::string>(bnode_output), (void*)&not_cl, false};
	nodetype* DFF_P = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), (void*)&dff_p_cl, true};
	nodetype* DFF_N = new nodetype{std::vector<std::string>(dff_inputs), std::vector<std::string>(dff_output), (void*)&dff_n_cl, true};
	nodetype* ADFF_PP1 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), (void*)&adff_pp1_cl, true};
	nodetype* ADFF_PP0 = new nodetype{std::vector<std::string>(adff_inputs), std::vector<std::string>(dff_output), (void*)&adff_pp1_cl, true};
	nodetype* DFFE_PP = new nodetype{std::vector<std::string>(dffe_inputs), std::vector<std::string>(dff_output), (void*)&dffe_pp_cl, true};
	nodetype* MUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), (void*)&mux_cl, false};
	nodetype* NMUX = new nodetype{std::vector<std::string>(mux_inputs), std::vector<std::string>(bnode_output), (void*)&nmux_cl, false};




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
		{"$_NMUX_", NMUX}
	};
	return nodetypes;
}

void and_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & *B;
}

struct starpu_codelet and_cl = {
#ifdef plain_mode
	.cpu_funcs = {and_plain},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void nand_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !( *A & *B);
}

struct starpu_codelet nand_cl = {
#ifdef plain_mode
	.cpu_funcs = {nand_plain},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void andnot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A & !*B;
}

struct starpu_codelet andnot_cl = {
#ifdef plain_mode
	.cpu_funcs = {andnot_plain},
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void or_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | *B;
}

struct starpu_codelet or_cl = {
#ifdef plain_mode
	.cpu_funcs = {or_plain},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void nor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A | *B);
}

struct starpu_codelet nor_cl = {
#ifdef plain_mode
	.cpu_funcs = {nor_plain},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void ornot_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A | ! *B;
}

struct starpu_codelet ornot_cl = {
#ifdef plain_mode
	.cpu_funcs = {ornot_plain},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void xor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = *A ^ *B;
}

struct starpu_codelet xor_cl = {
#ifdef plain_mode
	.cpu_funcs = {xor_plain},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void xnor_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[2]) = !(*A ^ *B);
}

struct starpu_codelet xnor_cl = {
#ifdef plain_mode
	.cpu_funcs = {xnor_plain},	
#endif
    .nbuffers = 3,
    .modes = {STARPU_R, STARPU_R, STARPU_W}
};

void not_plain(void *buffers[], void *cl_arg){
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[1]) = !*A;
}

struct starpu_codelet not_cl = {
#ifdef plain_mode
	.cpu_funcs = {not_plain},	
#endif
    .nbuffers = 2,
    .modes = {STARPU_R, STARPU_W}
};

void dff_p_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if(*CLK == true && *fCLK == false){
		*state = *D;
	}
	*fCLK = *CLK;
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[4]) = *state;
}

struct starpu_codelet dff_p_cl = {
#ifdef plain_mode
	.cpu_funcs = {dff_p_plain},	
#endif
    .nbuffers = 5,
    .modes = {STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_W}
};

void dff_n_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
	if(*CLK == false && *fCLK == true){
		*state = *D;
	}
	*fCLK = *CLK;
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[4]) = *state;
}

struct starpu_codelet dff_n_cl = {
#ifdef plain_mode
	.cpu_funcs = {dff_n_plain},	
#endif
    .nbuffers = 5,
    .modes = {STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_W}
};

void adff_pp1_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
	bool *fRST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	if(*CLK == true && *fCLK == false){
		*state = *D;
	}
	*fCLK = *CLK;
	if(*RST == true && *fRST == false){
		*state = true;
	}
	*fRST = *RST;
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[6]) = *state;
}

struct starpu_codelet adff_pp1_cl = {
#ifdef plain_mode
	.cpu_funcs = {adff_pp1_plain},	
#endif
    .nbuffers = 7,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R, STARPU_W}
};

void adff_pp0_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
	bool *fRST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	if(*CLK == true && *fCLK == false){
		*state = *D;
	}
	*fCLK = *CLK;
	if(*RST == true && *fRST == false){
		*state = false;
	}
	*fRST = *RST;
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[6]) = *state;
	}

struct starpu_codelet adff_pp0_cl = {
#ifdef plain_mode
	.cpu_funcs = {adff_pp0_plain},	
#endif
    .nbuffers = 7,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R, STARPU_W}
};

void dffe_pp_plain(void *buffers[], void *cl_arg){
	bool *state = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
	bool *fCLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]); //buf to detect edge
	bool *fRST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]); //buf to detect edge
    bool *D = (bool*)STARPU_VARIABLE_GET_PTR(buffers[3]);
    bool *CLK = (bool*)STARPU_VARIABLE_GET_PTR(buffers[4]);
    bool *RST = (bool*)STARPU_VARIABLE_GET_PTR(buffers[5]);
	bool *E = (bool*)STARPU_VARIABLE_GET_PTR(buffers[6]);
	if(*CLK == true && *fCLK == false && *E == true){
		*state = *D;
	}
	*fCLK = *CLK;
	if(*RST == true && *fRST == false){
		*state = false;
	}
	*fRST = *RST;
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[7]) = *state;
	}

struct starpu_codelet dffe_pp_cl = {
#ifdef plain_mode
	.cpu_funcs = {dffe_pp_plain},	
#endif
    .nbuffers = 8,
    .modes = {STARPU_RW, STARPU_RW, STARPU_RW, STARPU_R, STARPU_R, STARPU_R, STARPU_R, STARPU_W}
};

void mux_plain(void *buffers[], void *cl_arg){
    bool *S = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) = *S ? *A : *B;
}

struct starpu_codelet mux_cl = {
#ifdef plain_mode
	.cpu_funcs = {mux_plain},
#endif
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_W}
};

inline void nmux_plain(void *buffers[], void *cl_arg){
    bool *S = (bool*)STARPU_VARIABLE_GET_PTR(buffers[0]);
    bool *A = (bool*)STARPU_VARIABLE_GET_PTR(buffers[1]);
    bool *B = (bool*)STARPU_VARIABLE_GET_PTR(buffers[2]);
    *(bool*)STARPU_VARIABLE_GET_PTR(buffers[3]) = !*S ? *A : *B;
}

struct starpu_codelet nmux_cl = {
#ifdef plain_mode
	.cpu_funcs = {nmux_plain},	
#endif
    .nbuffers = 4,
    .modes = {STARPU_R, STARPU_R, STARPU_R, STARPU_W}
};