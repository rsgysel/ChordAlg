cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_gmock_test(ChordAlgTests atoms_graph_tests.cpp
                             bt_algorithm_tests.cpp
                             elimination_tests.cpp
                             file_reader_graph_tests.cpp
                             fill_edge_graph_tests.cpp
                             graph_tests.cpp
                             heuristic_run_tests.cpp
                             lb_triang_triangulation_tests.cpp
                             lex_trie_tests.cpp
                             mcs_triangulation_tests.cpp
                             mcs_m_triangulation_tests.cpp
                             minimal_separators_tests.cpp
                             potential_maxcliques_tests.cpp
                             separator_components_tests.cpp
                             separator_graph_tests.cpp
                             test_graphs.cpp
                             tree_tests.cpp
                             triangulation_filledges_tests.cpp
                             vertices_tests.cpp)
target_link_libraries(ChordAlgTests ChordAlgLib)

