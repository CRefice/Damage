if (WIN32)
	file (GLOB_RECURSE SOURCES "Win/*.cpp")
else ()
	file (GLOB_RECURSE SOURCES "Gtk/*.cpp")
endif (WIN32)

add_library (Gooey STATIC ${SOURCES})
