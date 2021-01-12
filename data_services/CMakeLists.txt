cmake_minimum_required(VERSION VERSION 3.16.3)
project(data_services)

add_library(data_services STATIC database_client.cpp redis_client.cpp)