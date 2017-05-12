file (GLOB_RECURSE SOURCES "*.cpp")

add_executable (Damage WIN32 ${SOURCES})
target_link_libraries(Damage libco Gooey gl)

if(WIN32)
	find_package(OpenGL)
	target_link_libraries (Damage ${OPENGL_LIBRARIES})
endif()

set_target_properties(Damage PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_include_directories(Damage PRIVATE ${PROJECT_SOURCE_DIR}/source ${PROJECT_SOURCE_DIR}/source/core)
