cmake_minimum_required(VERSION 3.17)
project(autopilot-engine)

include_directories(helpers helpers/expressions controllers data_services)

add_subdirectory(controllers)
add_subdirectory(data_services)
add_subdirectory(helpers/expressions)
add_subdirectory(helpers)

add_executable(main launch.cpp)

target_link_libraries(main controllers helpers expressions data_services curlpp curl pqxx)

