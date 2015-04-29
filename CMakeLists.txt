cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("ChordAlg")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()
include(gmock)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")
    set(debug_flags "-g")
    set(opt_flags "-O3")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(cxx_std "-std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(cxx_std "-std=c++11")
endif()
set(cxx_flags "${debug_flags} ${warnings} ${cxx_std}")
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${cxx_flags}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(ChordAlgSrc)
add_subdirectory(executables)
add_subdirectory(heuristics_experiments)
add_subdirectory(separatorgraph_experiments)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

message(STATUS "Current Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
