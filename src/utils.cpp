#include "backend.hpp"

std::vector<t_val> make_inputs_rand(
    std::map<std::string,int> inputs
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> plain{};
#else
    std::vector<uint8_t> plain{};
#endif
    std::random_device rnd;
    int r = 0;
    for(int t = 0; t < n; t++){
        for(auto it = inputs.begin(); it != inputs.end(); it++){
            int w = it->second;
            for(int i = 0; i < w;i++){
                if(r == 0) {
                    r = rnd();
                }
                plain.push_back((r & 1));
                std::cout << "input '" << it->first << "[" << i << "] : " << (r & 1) << std::endl;
                r = r >> 1;
            }
        }
    }
#ifdef plain_mode
    return plain;
#else
    return TFHEpp::bootsSymEncrypt<lvl_param>(plain, sk);
#endif
}

std::vector<t_val> make_inputs_manual(
    std::map<std::string,int> inputs
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> plain{};
#else
    std::vector<uint8_t> plain{};
#endif
    for(int t = 0; t < n; t++){
        for(auto it = inputs.begin(); it != inputs.end(); it++){
            int w = it->second;
            std::cout << "input '" << it->first << "[" << w << "]" << std::endl;
            int bits;
            std::cin >> bits;
            if(std::cin.fail()){
                std::cerr << "input error" << std::endl;
                return std::vector<t_val>{};
            }
            for(int i = 0; i < w;i++){
                plain.push_back((bits & 1));
                bits = bits >> 1;
            }
        }
    }
#ifdef plain_mode
    return plain;
#else
    return TFHEpp::bootsSymEncrypt<lvl_param>(plain, sk);
#endif
}



int result_dump(
    std::map<std::string,int> outputs,
    std::vector<t_val> retvals
#ifndef plain_mode
    , TFHEpp::SecretKey sk
#endif
    ,int n
){
#ifdef plain_mode
    std::vector<bool> result = retvals;
#else
    std::vector<uint8_t> result = TFHEpp::bootsSymDecrypt<lvl_param>(retvals, sk);
#endif
    int ret_counter = 0;
    for(int t = 0; t < n; t++){
        if(n > 1) std::cout << "CLK : " << t+1 << std::endl;
        for(auto it = outputs.begin(); it != outputs.end(); it++){
            for(int i = 0; i < it->second; i++){
                std::cout << "output '" << it->first <<"[" << i << "] : " << (result[ret_counter] ? "true": "false") << std::endl;
                ret_counter++;
                if(ret_counter > retvals.size()){
                    std::cerr << "err: too few retvals!" << std::endl;
                    return 1;
                }
            }
        }
    }
    return 0;
}
