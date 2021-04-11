cmake_minimum_required(VERSION 3.16.3)
project(helpers)

add_library(helpers STATIC conditions_parser.cpp http.cpp uuid.cpp)