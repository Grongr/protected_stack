cmake_minimum_required(VERSION 3.16)
project(protected_stack VERSION 1.0 LANGUAGES CXX)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNDEBUG")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wfloat-equal -Wsign-conversion -warray-bounds -wdiv-by-zero -wdouble-promotion")

add_executable(main main.cpp
                    stack.cpp
                    mem_copy.cpp)

target_link_libraries(main m)
