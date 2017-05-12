cmake_minimum_required (VERSION 3.0)
project (Gooey)

set (GOOEY_VERSION_MAJOR 0)
set (GOOEY_VERSION_MINOR 5)

include_directories(Include)
add_subdirectory (Source)
add_subdirectory (Tests)
