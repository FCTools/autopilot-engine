cmake_minimum_required(VERSION VERSION 2.8.12)
project(data_services)

add_library(data_services STATIC database_client.cpp redis_client.cpp)