cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_FLAGS "-march=native -O3 -g")
set(CMAKE_CXX_STANDARD 20)
project(starpu_tfhe CXX)




add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(thirdparties/TFHEpp)
#add_subdirectory(thirdparties/cuFHE/)


#add_compile_definitions(plain_mode)
add_compile_definitions(dump_mode)
#add_compile_definitions(use_simple_FF)
add_compile_definitions(perf_measure)
add_compile_definitions(use_lvl0)