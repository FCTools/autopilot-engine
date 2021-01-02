cmake_minimum_required(VERSION 3.17)
project(data_services)

add_library(data_services STATIC database_client.cpp redis_client.cpp)