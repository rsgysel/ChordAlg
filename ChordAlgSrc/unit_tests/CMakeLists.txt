cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_gmock_test(ChordAlgUnitTests file_reader_test.cpp
                                 graph_test.cpp
                                 lex_trie_test.cpp)
target_link_libraries(ChordAlgUnitTests ChordAlg)

