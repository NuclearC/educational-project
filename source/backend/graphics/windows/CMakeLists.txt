cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE WINDOWS_FILES ${SRC_DIR}/*.*)

add_library(emucore-windows ${WINDOWS_FILES})
set_property(TARGET emucore-windows PROPERTY CXX_STANDARD 20)
