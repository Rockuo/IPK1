cmake_minimum_required(VERSION 3.6)
project(IPK1)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES_SERVER server/main.c structures.h genericFunctions.c genericFunctions.h)
set(SOURCE_FILES_CLIENT client/main.c structures.h genericFunctions.c genericFunctions.h)
add_executable(ftrestd ${SOURCE_FILES_SERVER})
add_executable(ftrest ${SOURCE_FILES_CLIENT})