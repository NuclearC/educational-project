cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE OPENGL_DRIVER_FILES ${SRC_DIR}/*.*)

add_library(emucore-opengl-driver ${OPENGL_DRIVER_FILES})
set_property(TARGET emucore-opengl-driver PROPERTY CXX_STANDARD 20)
