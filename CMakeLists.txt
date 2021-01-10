cmake_minimum_required(VERSION 2.8.12)
project(autopilot-engine)

include_directories(/usr/local/include /usr/include)
include_directories(helpers helpers/expressions controllers data_services)
# include_directories(/home/alcatraz/acl/lib_acl_cpp/include /home/alcatraz/acl/lib_acl_cpp/include/acl_cpp)
# include_directories(/home/alcatraz/acl/lib_acl_cpp/lib /home/alcatraz/acl/lib_protocol/lib home/alcatraz/acl/lib_acl/lib)

add_subdirectory(controllers)
add_subdirectory(data_services)
add_subdirectory(helpers/expressions)
add_subdirectory(helpers)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DLINUX2 -g -W -O3 -Wall -Werror -Wshadow -Wno-long-long -Wpointer-arith -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/home/alcatraz/acl/lib_acl_cpp/lib -l_acl_cpp -L/home/alcatraz/acl/lib_protocol/lib -l_protocol -L/home/alcatraz/acl/lib_acl/lib -l_acl -lpthread")

add_executable(main launch.cpp)
target_link_libraries(main controllers helpers expressions data_services curlpp curl pqxx cpp_redis tacopie)

