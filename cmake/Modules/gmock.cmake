cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

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
# GMock
################################
set(GMOCK_DIR "/usr/src/gmock"
    CACHE PATH "The path to the GoogleMock test framework.")
add_subdirectory(${GMOCK_DIR} ${CMAKE_BINARY_DIR}/gmock)
set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")
include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

# add_gmock_test(<target> <sources>...)
# Adds a Googole Mock based test executable, <target>, built from <sources> and
# adds the test so that CTest will run it. Both the executable and the test
# will be named <target>.
function(add_gmock_test target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} gmock_main)
    add_test(${target} ${target})
    add_custom_command(TARGET ${target}
                       POST_BUILD
                       COMMAND ${target}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMENT "Running ${target}" VERBATIM)
endfunction()
