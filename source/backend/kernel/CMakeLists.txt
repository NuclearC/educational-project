cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE KERNEL_FILES ${SRC_DIR}/*.*)

add_library(emucore-kernelimpl ${KERNEL_FILES})
set_property(TARGET emucore-kernelimpl PROPERTY CXX_STANDARD 20)