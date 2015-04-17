cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_gmock_test(ChordAlgUnitTests atoms_graph_integration_tests.cpp
                                 file_reader_graph_integration_tests.cpp
                                 file_reader_unit_tests.cpp
                                 graph_unit_tests.cpp
                                 lex_trie_unit_tests.cpp
                                 mcs_triangulation_integration_tests.cpp
                                 minimal_separator_algorithms_integration_tests.cpp
                                 separator_components_integration_tests.cpp
                                 separator_graph_integration_tests.cpp
                                 test_graphs.cpp
                                 vertices_unit_tests.cpp)
target_link_libraries(ChordAlgUnitTests ChordAlgLib)

