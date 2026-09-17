cmake_minimum_required(VERSION 3.16)
project(bread_engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(bread_SRC "${CMAKE_CURRENT_SOURCE_DIR}")

# Auto-download sse2neon.h if compiling on ARM or missing
if(NOT EXISTS "${bread_SRC}/extern/sse2neon.h")
    message(STATUS "Downloading sse2neon.h header...")
    file(DOWNLOAD 
        "https://raw.githubusercontent.com/DLu/sse2neon/master/sse2neon.h"
        "${bread_SRC}/extern/sse2neon.h"
        SHOW_PROGRESS
    )
endif()

set(bread_ARCH "native" CACHE STRING "Target CPU architecture")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (bread_ARCH STREQUAL "arm64" OR bread_ARCH STREQUAL "armv8-a")
        add_compile_options(-march=armv8-a)
    else()
        add_compile_options(-march=${bread_ARCH})
    endif()
    add_compile_options(-flto=thin)
    add_link_options(-flto=thin -fuse-ld=lld)
endif()

include_directories(${bread_SRC})

add_executable(bread_engine
    nnue.cpp
)
