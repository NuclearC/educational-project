cmake_minimum_required(VERSION 3.10)
project(emulator)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

file(GLOB_RECURSE FILES ${PROJECT_SOURCE_DIR}/source/*.*)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/asmjit)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/zydis)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/libui)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIRS ${PROJECT_BINARY_DIR}/third-party/zydis/dependencies/zycore ${PROJECT_SOURCE_DIR}/source/
 ${THIRD_PARTY_DIR}/zydis/include/ ${THIRD_PARTY_DIR}/zydis/dependencies/zycore/include/ ${THIRD_PARTY_DIR}/asmtk/src/ ${THIRD_PARTY_DIR}/asmjit/src/ ${THIRD_PARTY_DIR}/libui/)

if(WIN32)

set(THIRD_PARTY_LIBS asmjit Zydis libui user32 kernel32 gdi32 comctl32 uxtheme msimg32 comdlg32 d2d1 dwrite ole32 oleaut32 oleacc uuid windowscodecs)

else()

set(THIRD_PARTY_LIBS asmjit Zydis libui)

endif()

add_executable(emulator ${FILES})
target_include_directories(emulator PUBLIC ${INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(emulator ${THIRD_PARTY_LIBS} ${OPENGL_LIBRARIES})
set_property(TARGET emulator PROPERTY CXX_STANDARD 20)