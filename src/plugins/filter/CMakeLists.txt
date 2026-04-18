cmake_minimum_required(VERSION 3.0)

set(FILTER_PLUGINS "")

# Sample filter plugin
add_library(filter_sample_filter MODULE sample_filter.c)
target_link_libraries(filter_sample_filter fluent-bit)
set_target_properties(filter_sample_filter PROPERTIES PREFIX "")
list(APPEND FILTER_PLUGINS filter_sample_filter)

# Set output directory for all plugins
foreach(plugin ${FILTER_PLUGINS})
    set_target_properties(${plugin} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
