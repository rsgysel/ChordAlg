cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(basic_test)

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

################################
# GTest
################################
add_subdirectory(/usr/src/gtest ${CMAKE_BINARY_DIR}/gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable(runUnitTests sal_fr_test.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main ChordAlg)
add_test(runUnitTests runUnitTests)
