cmake_minimum_required(VERSION 3.20)
project(bootlang LANGUAGES CXX)

# Require modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect source files
file(GLOB_RECURSE BOOTLANG_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    include/*.hpp
    src/nodes/*.cpp
    include/nodes/*.hpp
)

# Define the library
add_library(bootlang ${BOOTLANG_SOURCES})

# Specify include directories for users of the library
target_include_directories(bootlang
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable strict warnings
if (MSVC)
    target_compile_options(bootlang PRIVATE /W4 /permissive-)
else()
    target_compile_options(bootlang PRIVATE -Wall -Wextra -Wpedantic)
endif()
