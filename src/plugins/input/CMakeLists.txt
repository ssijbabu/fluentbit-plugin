cmake_minimum_required(VERSION 3.0)

set(INPUT_PLUGINS "")

# Sample input plugin
add_library(input_sample_input MODULE sample_input.c)
target_link_libraries(input_sample_input fluent-bit)
set_target_properties(input_sample_input PROPERTIES PREFIX "")
list(APPEND INPUT_PLUGINS input_sample_input)

# Set output directory for all plugins
foreach(plugin ${INPUT_PLUGINS})
    set_target_properties(${plugin} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
