cmake_minimum_required(VERSION 3.16.3)
project(conditions-runner)
SET(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/include /usr/include)
include_directories(../../helpers/conditions)
include_directories(../../helpers)

add_library(conditions STATIC ../../helpers/conditions/complex_condition.cpp ../../helpers/conditions/elementary_condition.cpp ../../helpers/conditions/base_condition.cpp ../../helpers/conditions_parser.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g")

add_executable(runner runner.cpp)
target_link_libraries(runner conditions)
