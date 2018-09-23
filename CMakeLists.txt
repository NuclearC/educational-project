cmake_minimum_required(VERSION 3.10)
project(emulator)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE FILES ${PROJECT_SOURCE_DIR}/source/*.*)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/asmjit)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/zydis)

add_executable(emulator ${FILES})
target_include_directories(emulator PUBLIC ${PROJECT_SOURCE_DIR}/source/ ${PROJECT_SOURCE_DIR}/third-party/zydis/include/ ${PROJECT_SOURCE_DIR}/third-party/asmtk/src/ ${PROJECT_SOURCE_DIR}/third-party/asmjit/src/)
target_link_libraries(emulator asmjit Zydis)