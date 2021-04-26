cmake_minimum_required(VERSION 3.16.3)
project(utils)

add_library(utils STATIC conditions_parser.cpp http.cpp uuid.cpp time_.cpp utils.cpp)