cmake_minimum_required(VERSION VERSION 3.16.3)
project(tracker_controllers)


add_library(tracker_controllers STATIC binom_client.cpp)
