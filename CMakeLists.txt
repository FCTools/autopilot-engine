cmake_minimum_required(VERSION 2.8.12)
project(autopilot-engine)

include_directories(/usr/local/include /usr/include)
include_directories(helpers helpers/expressions controllers data_services)
include_directories(/home/alcatraz/acl/lib_acl_cpp/include)

add_subdirectory(controllers)
add_subdirectory(data_services)
add_subdirectory(helpers/expressions)
add_subdirectory(helpers)

add_executable(main launch.cpp)

add_compile_options(-DLINUX2 -L /home/alcatraz/acl/lib_acl_cpp/lib -l_acl_cpp -L /home/alcatraz/acl/lib_protocol/lib -l_protocol -L /home/alcatraz/acl/lib_acl/lib -l_acl   -lpthread)
target_link_libraries(main controllers helpers expressions data_services curlpp curl pqxx)

