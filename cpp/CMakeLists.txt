cmake_minimum_required(VERSION 3.20)
project(BootLangTest LANGUAGES CXX)

# Set latest stable C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict compiler warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add the BootLang library
add_subdirectory(bootlang)

# Main executable
add_executable(BootLangTest main.cpp)

# Link against the BootLang library
target_link_libraries(BootLangTest PRIVATE bootlang)