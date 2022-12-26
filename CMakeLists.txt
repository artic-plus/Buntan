cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_FLAGS "-march=native -O3 -g")
set(CMAKE_CXX_STANDARD 20)
project(starpu_tfhe CXX)




add_subdirectory(src)
add_subdirectory(thirdparties/cuFHE/thirdparties/TFHEpp)
#add_subdirectory(thirdparties/cuFHE/)