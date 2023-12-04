cmake_minimum_required(VERSION 3.22)
project(
    Eagle
    DESCRIPTION A DSA library for C
    HOMEPAGE_URL https://github.com/kai-patel/eagle
    LANGUAGES C
    )

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS -Wall -Wextra)

set(EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_CLANG_TIDY clang-tidy -checks=-*,bugprone-*,performance-*,misc-*)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in any directory with a CMakeLists.txt file")
endif()

include(FetchContent)

add_subdirectory(src)
add_subdirectory(test)

