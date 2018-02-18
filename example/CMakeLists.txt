set(library_project_name "${PROJECT_NAME}")

set(example_files
    "${CMAKE_CURRENT_SOURCE_DIR}/quickstart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.c"
)

foreach(file ${example_files})
    get_filename_component(filenamewe "${file}" NAME_WE)
    add_executable("${filenamewe}.${library_project_name}.example" "${file}")
    set_target_properties("${filenamewe}.${library_project_name}.example"
        PROPERTIES
            # CXX_STANDARD "${CMAKE_CXX_STANDARD}"
            # CXX_STANDARD_REQUIRED ON
            # CXX_EXTENSIONS OFF
            # NO_SYSTEM_FROM_IMPORTED ON
            COMPILE_FLAGS "-pedantic -Wall -Wextra -Werror"
    )
    target_link_libraries("${filenamewe}.${library_project_name}.example" PUBLIC "${library_project_name}_single_include")
endforeach()