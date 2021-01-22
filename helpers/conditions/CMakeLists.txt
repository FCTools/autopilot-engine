cmake_minimum_required(VERSION 3.16.3)
project(conditions)

add_library(conditions STATIC complex_condition.cpp elementary_condition.cpp base_condition.cpp)