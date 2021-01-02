cmake_minimum_required(VERSION 3.17)
project(controllers)

add_library(controllers STATIC base_controller.cpp propeller_controller.cpp)