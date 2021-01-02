cmake_minimum_required(VERSION 3.17)
project(expressions)

add_library(expressions STATIC binary_operation.cpp elementary_condition.cpp expression.cpp)