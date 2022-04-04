cmake_minimum_required(VERSION 3.9)
project(hello_cpp_http)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)

find_package(PkgConfig)

pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)
add_executable(httpserver_run ${SOURCE_FILES})

target_link_libraries(httpserver_run PkgConfig::Pistache)