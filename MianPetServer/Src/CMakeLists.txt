cmake_minimum_required(VERSION 2.8)

project(MianPetServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O3)

link_libraries(pthread)

include_directories(./asio-1.16.0/)

aux_source_directory(. DIR_SRSC)

add_executable(MianPetServer ${DIR_SRSC})
