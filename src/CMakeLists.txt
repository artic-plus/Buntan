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

add_executable(runtime_unode runtime_unode.cpp nodetypes.cpp frontend.cpp backend.cpp)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/include)
target_include_directories(runtime_unode PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(runtime_unode PUBLIC tfhe++)

add_executable(client client.cpp frontend.cpp backend.cpp)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(client PUBLIC tfhe++)


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


add_executable(check_and check_and.cpp nodetypes.cpp frontend.cpp backend.cpp)
target_include_directories(check_and PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(check_and PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/picojson)
target_include_directories(check_and PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cereal/include)
target_include_directories(check_and PUBLIC ${CMAKE_SOURCE_DIR}/thirdparties/cuFHE/thirdparties/TFHEpp)

target_link_libraries(check_and PUBLIC tfhe++)

add_compile_definitions(plain_mode)
#add_compile_definitions(use_simple_FF)