project(dsunExamples)

include_directories(${CMAKE_SOURCE_DIR}../src)


file(GLOB EXAMPLE_SOURCES "*.cpp")


foreach(example_src ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    add_dependencies(${example_name} dsun)

    target_link_libraries(${example_name} dsun)  
endforeach()