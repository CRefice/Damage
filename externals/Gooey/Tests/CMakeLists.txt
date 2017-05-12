add_executable(ControlTest ControlTest.cpp)
target_link_libraries(ControlTest Gooey)

if(WIN32) # Check if we are on Windows
	set_target_properties(ControlTest PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

target_include_directories(ControlTest PRIVATE ${PROJECT_SOURCE_DIR}/Include)
