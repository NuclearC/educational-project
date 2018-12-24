cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE CPU_FILES ${SRC_DIR}/*.*)

add_library(emucore-cpu ${CPU_FILES})
message (${PROJECT_BINARY_DIR}/third-party/zydis/dependencies/zycore)
set_property(TARGET emucore-cpu PROPERTY CXX_STANDARD 20)