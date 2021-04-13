cmake_minimum_required(VERSION VERSION 3.16.3)
project(ts_controllers)


add_library(ts_controllers STATIC base_controller.cpp propeller_controller.cpp evadav_controller.cpp mgid_controller.cpp kadam_controller.cpp)
