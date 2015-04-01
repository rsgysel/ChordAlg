cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_gmock_test(FileReaderTest file_reader_test.cpp)
target_link_libraries(FileReaderTest ChordAlg)

