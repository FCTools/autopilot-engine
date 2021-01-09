cmake_minimum_required(VERSION VERSION 2.8.12)
project(controllers)

add_library(controllers STATIC base_controller.cpp propeller_controller.cpp)