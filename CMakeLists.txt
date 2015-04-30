cmake_minimum_required (VERSION 2.6)
project(SMC2015)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules/")

find_package(MathSAT REQUIRED)
find_package(GMP REQUIRED)

add_library(sexpr
  sexpr-1.3/src/sexp.c
  sexpr-1.3/src/parser.c
  sexpr-1.3/src/io.c
  sexpr-1.3/src/faststack.c
  sexpr-1.3/src/sexp_ops.c
  sexpr-1.3/src/sexp_vis.c
  sexpr-1.3/src/cstring.c
  sexpr-1.3/src/sexp_memory.c
  sexpr-1.3/src/event_temp.c)

add_executable(bmc main.cc bmc_exercise.cc mathsat_interface.cc parser.cc program.cc)
target_link_libraries(bmc ${MATHSAT_LIBRARY} ${GMP_LIBRARY} sexpr)
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wno-sign-compare")

include_directories(${MATHSAT_INCLUDE_DIR})
include_directories(sexpr-1.3/src)
