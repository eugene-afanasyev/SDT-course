cmake_minimum_required(VERSION 3.17)
project(hash-map-eugene-afanasyev)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(--coverage)
    link_libraries(gcov)
endif ()

add_executable(main test.cpp hash_map.hpp)