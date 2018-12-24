cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE GFX_FILES ${SRC_DIR}/*.*)

add_library(emucore-graphics ${GFX_FILES})
set_property(TARGET emucore-graphics PROPERTY CXX_STANDARD 20)

add_subdirectory(${SRC_DIR}/drivers)
add_subdirectory(${SRC_DIR}/windows)