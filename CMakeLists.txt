cmake_minimum_required(VERSION 3.6)
project(IPK1)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES_SERVER server/main.c)
set(SOURCE_FILES_CLIENT client/main.c)
add_executable(IPK1_server ${SOURCE_FILES_SERVER})
add_executable(IPK1_client ${SOURCE_FILES_CLIENT})