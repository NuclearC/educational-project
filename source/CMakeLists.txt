cmake_minimum_required(VERSION 3.10)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB APP_FILES 
  "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp"
  "${SRC_DIR}/utils/*.cpp" "${SRC_DIR}/utils/*.hpp"
  "${SRC_DIR}/gui/*.cpp" "${SRC_DIR}/gui/*.hpp"
  "${SRC_DIR}/elf/*.cpp" "${SRC_DIR}/elf/*.hpp")

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

set(INCLUDE_DIRS ${PROJECT_BINARY_DIR}/third-party/zydis ${PROJECT_BINARY_DIR}/third-party/zydis/dependencies/zycore ${PROJECT_SOURCE_DIR}/source/
 ${THIRD_PARTY_DIR}/zydis/include/ ${THIRD_PARTY_DIR}/zydis/dependencies/zycore/include/ ${THIRD_PARTY_DIR}/asmtk/src/ ${THIRD_PARTY_DIR}/asmjit/src/ ${THIRD_PARTY_DIR}/libui/)

if(WIN32)

  set(THIRD_PARTY_LIBS asmjit Zydis libui user32 kernel32 gdi32 comctl32 uxtheme msimg32 comdlg32 d2d1 dwrite ole32 oleaut32 oleacc uuid windowscodecs)

else()

  set(THIRD_PARTY_LIBS asmjit Zydis libui)

endif()

include_directories(${INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

add_subdirectory(${SRC_DIR}/backend)

add_executable(emulator ${APP_FILES})
target_link_libraries(emulator ${THIRD_PARTY_LIBS} ${OPENGL_LIBRARIES}
  emucore-audio emucore-cpu emucore-graphics emucore-memory emucore-opengl-driver emucore-windows emucore-kernelimpl)
set_property(TARGET emulator PROPERTY CXX_STANDARD 20)
