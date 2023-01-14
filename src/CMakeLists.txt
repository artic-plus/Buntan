cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_FLAGS "-march=native -O3 -g")
set(CMAKE_CXX_STANDARD 20)
project(starpu_tfhe CXX)

find_package(OpenMP)
if(OpenMP_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(PkgConfig)
pkg_check_modules(STARPU REQUIRED starpu-1.3)
if (STARPU_FOUND)
include_directories (${STARPU_INCLUDE_DIRS})
link_directories (${STARPU_STATIC_LIBRARY_DIRS})
link_libraries (${STARPU_STATIC_LIBRARIES})
else (STARPU_FOUND)
message(FATAL_ERROR "StarPU not found")
endif()
pkg_check_modules(STARPU_MPI REQUIRED starpumpi-1.3)
if (STARPU_MPI_FOUND)
include_directories (${STARPU_MPI_INCLUDE_DIRS})
link_directories (${STARPU_MPI_STATIC_LIBRARY_DIRS})
link_libraries (${STARPU_MPI_STATIC_LIBRARIES})
else (STARPU_MPI_FOUND)
message(FATAL_ERROR "StarPU_MPI not found")
endif()


find_package(MPI)

add_executable(runtime_unode runtime_unode.cpp nodetypes.cpp frontend.cpp backend.cpp)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(runtime_unode PUBLIC tfhe++)

target_link_libraries(runtime_unode PUBLIC MPI::MPI_CXX)
target_compile_options(runtime_unode PRIVATE -lstarpu-1.3 -lstarpumpi-1.3 -lmpi -Wall)
target_link_options(runtime_unode PRIVATE -lstarpu-1.3 -lstarpumpi-1.3 -lmpi -Wall)


add_executable(runtime_mpi runtime_mpi.cpp nodetypes.cpp frontend.cpp backend.cpp backend_mpi.cpp)
target_include_directories(runtime_mpi PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runtime_mpi PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(runtime_mpi PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(runtime_mpi PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/include)
target_include_directories(runtime_mpi PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(runtime_mpi PUBLIC tfhe++)


target_link_libraries(runtime_mpi PUBLIC MPI::MPI_CXX)
target_compile_options(runtime_mpi PRIVATE -lstarpu-1.3 -lstarpumpi-1.3 -lmpi -Wall -rdynamic)
target_link_options(runtime_mpi PRIVATE -lstarpu-1.3 -lstarpumpi-1.3 -lmpi -Wall)



add_executable(client client.cpp frontend.cpp backend.cpp)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(client PUBLIC tfhe++)


add_executable(client_manual client_manual.cpp frontend.cpp backend.cpp)
target_include_directories(client_manual PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(client_manual PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(client_manual PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(client_manual PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(client_manual PUBLIC tfhe++)


add_executable(verify verify.cpp frontend.cpp backend.cpp)
target_include_directories(verify PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(verify PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(verify PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(verify PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(verify PUBLIC tfhe++)


add_executable(dump dump.cpp nodetypes.cpp frontend.cpp backend.cpp)
target_include_directories(dump PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(dump PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(dump PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(dump PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(dump PUBLIC tfhe++)


target_link_libraries(dump PUBLIC MPI::MPI_CXX)


#add_compile_definitions(plain_mode)
add_compile_definitions(dump_mode)
#add_compile_definitions(use_simple_FF)
add_compile_definitions(perf_measure)
add_compile_definitions(use_lvl0)