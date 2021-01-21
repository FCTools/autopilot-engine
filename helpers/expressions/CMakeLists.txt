cmake_minimum_required(VERSION 3.16.3)
project(expressions)

add_library(expressions STATIC complex_condition.cpp elementary_condition.cpp expression.cpp)