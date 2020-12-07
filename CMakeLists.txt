# Standard CMake header config.
cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_CXX_STANDARD 11)

#==============================================================================

add_library(microcbor INTERFACE)
target_include_directories(microcbor INTERFACE include/)
