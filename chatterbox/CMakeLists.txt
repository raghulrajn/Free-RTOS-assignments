cmake_minimum_required(VERSION 3.24)

# (e.g. use cmake '--DEXAMPLE=1 ..' to build the example and 'cmake ..' to build the assignment

if (EXAMPLE)
    project(example)
	
	message(STATUS "build example code")

    add_subdirectory(src)
    add_subdirectory(example)

    target_link_libraries(example freertos_kernel)
else ()
    project(chatterbox_app)
	
	message(STATUS "build assignment code")

    add_subdirectory(src)
    add_subdirectory(chatterbox_app)

    target_link_libraries(chatterbox_app freertos_kernel)
endif ()
 
