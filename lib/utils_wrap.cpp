
#include "nodetypes.hpp"
#include <tfhe++.hpp>
#include <vector>

std::vector<bool_enc> enc_vec(std::vector<uint8_t> plain_vec, seckey sk){
        return TFHEpp::bootsSymEncrypt<lvl_param>(plain_vec, sk);
}
std::vector<uint8_t> dec_vec(std::vector<bool_enc> cipher_vec, seckey sk){
        return TFHEpp::bootsSymDecrypt<lvl_param>(cipher_vec, sk);
}

std::unique_ptr<seckey> gen_key(evalkey &ek){
    std::unique_ptr<seckey> sk(new seckey);
    ek.emplacebkfft<TFHEpp::lvl01param>(*sk);
    ek.emplaceiksk<TFHEpp::lvl10param>(*sk);
    return sk;
}