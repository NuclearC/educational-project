cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE MEMORY_FILES ${SRC_DIR}/*.*)

add_library(emucore-memory ${MEMORY_FILES})
set_property(TARGET emucore-memory PROPERTY CXX_STANDARD 20)