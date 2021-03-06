cmake_minimum_required(VERSION 3.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(server_and_client)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include)

add_library(server source/server.cpp include/server.h)
add_library(client source/client.cpp include/client.h)
add_library(telemetry source/telemetry.cpp include/telemetry.h)

add_executable(server_app application/server_app.cpp)
add_executable(client_app application/client_app.cpp)

target_link_libraries(server_app server)
target_link_libraries(client_app client)

target_link_libraries(server telemetry)
target_link_libraries(client telemetry)

find_package(Qt5Widgets CONFIG REQUIRED)

qt5_use_modules(telemetry Core Network)
qt5_use_modules(server Widgets Network)
qt5_use_modules(client Widgets Network)
