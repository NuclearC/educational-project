cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE AUDIO_FILES ${SRC_DIR}/*.*)

add_library(emucore-audio ${AUDIO_FILES})
set_property(TARGET emucore-audio PROPERTY CXX_STANDARD 20)