cmake_minimum_required(VERSION 3.12.4)  

# Arduino Library Template 
set(lib arduino-lib-template)

project(${lib} CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)

# Coverage Flags
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")

# Header & Source Files
set(Headers
    src/library-ino.h
    src/library/library-types.h
    src/library/library-pal.h
    src/library/library.h
    src/utils/utility.h
    )

set(Sources
    src/library-ino.cpp
    src/library/library.c
    src/utils/utility.cpp
    )

# Add Library
add_library(${lib} ${Sources} ${Headers})

# Enable Testing with CTest
enable_testing()

# Add GoogleTest 
add_subdirectory(googletest)
# Add Tests Folder
add_subdirectory(test)
