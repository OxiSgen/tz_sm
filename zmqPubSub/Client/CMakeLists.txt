cmake_minimum_required(VERSION 3.23.0)
project(zmq_ps)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.78 COMPONENTS program_options REQUIRED)
add_executable(zmq_ps src/client.cpp)

target_link_libraries(zmq_ps Boost::program_options)

target_link_libraries(zmq_ps zmq)

