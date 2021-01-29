cmake_minimum_required(VERSION 3.16.3)
project(conditions-runner)

include_directories(/usr/local/include /usr/include)
include_directories(~/projects/autopilot-engine/helpers/conditions)
include_directories(~/projects/autopilot-engine/helpers)

add_library(conditions STATIC ../../helpers/conditions/complex_condition.cpp ../../helpers/conditions/elementary_condition.cpp ../../helpers/conditions/base_condition.cpp ../../helpers/conditions_parser.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")

add_executable(runner runner.cpp)
target_link_libraries(runner conditions)
