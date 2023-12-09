# Describe project build

cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(ParProg VERSION 0.1 LANGUAGES C)

find_package(MPI REQUIRED)
find_package(Python3 REQUIRED)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")

set(OUTPUT_LAB01_DIR ${PROJECT_BINARY_DIR}/lab01)
file(MAKE_DIRECTORY ${OUTPUT_LAB01_DIR})

add_subdirectory(lab01)
