cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_FLAGS "-march=native -O3 -g")
set(CMAKE_CXX_STANDARD 20)
project(starpu_tfhe CXX)

option(plain_mode "evaluate on plain binary" OFF)
option(dump_mode "show debug info" OFF)
option(use_simple_FF "ignore clock inputs of FF" ON)
option(perf_measure "measure and show duration of procesing" ON)
option(use_lvl0 "use TLWElvl0 as a format of coded message" OFF)

add_subdirectory(src)
add_subdirectory(lib)
set(ENABLE_SHARED ON  CACHE BOOL "Build as shared libraries" FORCE)
add_subdirectory(thirdparties/TFHEpp)
#add_subdirectory(thirdparties/cuFHE/)

