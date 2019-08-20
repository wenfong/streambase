cmake_minimum_required(VERSION "3.7.1")

project(streambase)

add_executable(client src/client.cpp)
add_executable(server src/server.cpp src/user.cpp src/user.h)
