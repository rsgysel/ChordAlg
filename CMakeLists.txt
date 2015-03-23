cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("ChordAlg")

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cxx_std "-std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cxx_std "-std=c++11")
endif()
set(cxx_flags "${warnings} ${cxx_std}")
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${cxx_flags}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)



set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

message(STATUS "Current Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")