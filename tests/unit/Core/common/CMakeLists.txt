cmake_minimum_required(VERSION 2.8.11)
project(COMMON_TESTS)
set(CX_SRC "../../../../src/")
set(CX_INCL "../../../../include/")

include_directories(${CX_INCL})
file (GLOB src_common ${CX_SRC}core/common/*.cpp ${CX_INCL}/core/common/*.h)
file (GLOB src_threading ${CX_SRC}core/threading/*.cpp ${CX_INCL}/core/threading/*.h)
file (GLOB src_tests *.cpp)

foreach(test_src ${src_tests})
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src} ${src_common} ${src_threading})
endforeach()
