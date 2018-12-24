cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${SRC_DIR}/audio)
add_subdirectory(${SRC_DIR}/cpu)
add_subdirectory(${SRC_DIR}/graphics)
add_subdirectory(${SRC_DIR}/memory)
