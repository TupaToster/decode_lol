cmake_minimum_required(VERSION 3.19)

project(rofl-cpu)

include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(cpu)

foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

