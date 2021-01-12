cmake_minimum_required(VERSION 3.16.3)
project(autopilot-engine)

include_directories(/usr/local/include /usr/include)
include_directories(helpers helpers/expressions controllers data_services)

add_subdirectory(data_services)
add_subdirectory(controllers)
add_subdirectory(helpers)
add_subdirectory(helpers/expressions)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DLINUX2 -g -W -O3 -Wall -Wshadow -Wno-long-long -Wpointer-arith -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS")

add_executable(main launch.cpp main_loop.cpp)
target_link_libraries(main curlpp curl pqxx cpp_redis tacopie controllers data_services helpers expressions)

