﻿cmake_minimum_required(VERSION 2.8)

project(MianPetServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

link_libraries(pthread)

include_directories(./Src/asio-1.18.1)
include_directories(./Src/simdjson-0.8.0) #repeated
include_directories(./Src/otl-4.0.459)
include_directories(./Src/Config)

aux_source_directory(./Src DIR_SRSC)
aux_source_directory(./Src/simdjson-0.8.0 SIMDJSON_SRC) #repeated

add_executable(MianPetServer ${SIMDJSON_SRC} ${DIR_SRSC})
target_link_libraries(MianPetServer libmyodbc8a.so)
