cmake_minimum_required(VERSION 3.10)
project(emulator)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/asmjit)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/zydis)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/libui)

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)
