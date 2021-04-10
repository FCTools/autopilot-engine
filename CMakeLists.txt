cmake_minimum_required(VERSION 3.16.3)
project(autopilot-engine)
SET(CMAKE_CXX_STANDARD 11)

# standard ubuntu directories
include_directories(/usr/local/include /usr/include /usr/local/include/pqxx)

# standard mac os directories
include_directories(/opt/homebrew/opt/libpqxx@6/include /opt/homebrew/opt/curlpp/include)

link_directories(/opt/homebrew/opt/libpqxx@6/lib /opt/homebrew/opt/curlpp/lib)
link_directories(/usr/local/lib)

include_directories(helpers helpers/conditions controllers data_services)

add_subdirectory(data_services)
add_subdirectory(controllers)
add_subdirectory(helpers)
add_subdirectory(helpers/conditions)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g")

add_executable(main launch.cpp main_loop.cpp)
target_link_libraries(main controllers data_services helpers conditions pqxx curlpp curl cpp_redis tacopie spdlog)
