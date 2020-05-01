cmake_minimum_required(VERSION 2.8)

project(MianPetServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

link_libraries(pthread)

include_directories(./Src/asio-1.16.0/)

aux_source_directory(./Src/ DIR_SRSC)

add_executable(MianPetServer ${DIR_SRSC})
