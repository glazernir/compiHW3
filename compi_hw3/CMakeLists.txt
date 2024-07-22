cmake_minimum_required(VERSION 3.21)
project(compi_hw3)

set(CMAKE_CXX_STANDARD 14)

add_executable(compi_hw3 main.cpp hw3_output.cpp hw3_output.hpp SymbolTable.cpp SymbolTable.h scanner.lex parser.ypp)
