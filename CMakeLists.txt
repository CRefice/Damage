cmake_minimum_required (VERSION 3.5)
project (Damage)

include_directories(externals)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(externals)
add_subdirectory(source)
