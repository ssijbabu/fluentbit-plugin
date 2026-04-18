cmake_minimum_required(VERSION 3.10)
project(fluentbit-plugins C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2 -fPIC")

# Fluent Bit source and installation paths
set(FLUENT_BIT_INCLUDE_DIR "/usr/local/include" CACHE STRING "Fluent Bit include directory")
set(FLUENT_BIT_SRC_DIR "/fluent-bit-src" CACHE STRING "Fluent Bit source directory")
set(FLUENT_BIT_LIB_DIR "/usr/local/lib" CACHE STRING "Fluent Bit lib directory")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${FLUENT_BIT_INCLUDE_DIR}
    ${FLUENT_BIT_SRC_DIR}
    ${FLUENT_BIT_SRC_DIR}/include
    ${FLUENT_BIT_SRC_DIR}/lib/monkey/include
)

link_directories(${FLUENT_BIT_LIB_DIR})

# Enable subdirectories
add_subdirectory(src/plugins/input)
add_subdirectory(src/plugins/output)
add_subdirectory(src/plugins/filter)
