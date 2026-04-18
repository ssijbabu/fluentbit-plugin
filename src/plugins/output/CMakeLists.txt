cmake_minimum_required(VERSION 3.0)

set(OUTPUT_PLUGINS "")

# Sample output plugin
add_library(output_sample_output MODULE sample_output.c)
target_link_libraries(output_sample_output fluent-bit)
set_target_properties(output_sample_output PROPERTIES PREFIX "")
list(APPEND OUTPUT_PLUGINS output_sample_output)

# Set output directory for all plugins
foreach(plugin ${OUTPUT_PLUGINS})
    set_target_properties(${plugin} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
