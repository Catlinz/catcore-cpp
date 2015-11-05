# core/io/file Tests
cmake_minimum_required(VERSION 2.8.11)
project(CORE_IO_FILE_TESTS)
set(CX_SRC "../../../../../src/")
set(CX_INCL "../../../../../include/")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

include_directories(${CX_INCL})
file (GLOB src_core ${CX_SRC}core/*.cpp ${CX_INCL}core/*.h)
file (GLOB src_common ${CX_SRC}core/common/*.cpp ${CX_INCL}/core/common/*.h)
file (GLOB src_io ${CX_SRC}core/io/*.cpp ${CX_INCL}/core/io/*.h)
file (GLOB src_file ${CX_SRC}core/io/file/*.cpp ${CX_INCL}/core/io/file/*.h)
file (GLOB src_tests *.cpp)

foreach(test_src ${src_tests})
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src} ${src_core} ${src_common} ${src_io} ${src_file})
endforeach()
