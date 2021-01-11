cmake_minimum_required(VERSION 2.8.12)
project(helpers)

add_library(helpers STATIC conditions_parser.cpp config_reader)