
# The first line states the minimum CMake version to use with this project
# and is requires by CMake.
cmake_minimum_required(VERSION 3.0.0)

# Set <PROJNAME> to be your project name.
project(hw1 VERSION 0.1.0)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc

set(MTM_FLAGS_DEBUG "--std=c++11 -Wall -pedantic-errors -Werror -g")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

# Set the flags for gcc c++
set(CMAKE_CXX_FLAGS ${MTM_FLAGS_DEBUG})

# add_executable(prog tests.cpp recordsCompany.cpp)
add_executable(prog mainWet2.cpp recordsCompany.cpp)

