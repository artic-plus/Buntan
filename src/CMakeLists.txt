cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_FLAGS "-march=native -O3 -g")
set(CMAKE_CXX_STANDARD 20)
project(starpu_tfhe CXX)

find_package(PkgConfig)
pkg_check_modules(STARPU REQUIRED starpu-1.3)
if (STARPU_FOUND)
include_directories (${STARPU_INCLUDE_DIRS})
link_directories (${STARPU_STATIC_LIBRARY_DIRS})
link_libraries (${STARPU_STATIC_LIBRARIES})
else (STARPU_FOUND)
message(FATAL_ERROR "StarPU not found")
endif()

add_executable(a.out main.cpp frontend.cpp nodetypes.cpp)
target_include_directories(a.out PUBLIC ${CMAKE_SOURCE_DIR}/include)



add_compile_definitions(plain_mode)