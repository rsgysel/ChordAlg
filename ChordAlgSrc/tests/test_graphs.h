#ifndef INCLUDE_TEST_GRAPHS_H_
#define INCLUDE_TEST_GRAPHS_H_

#include <vector>

#include "ChordAlgSrc/vertex_utilities.h"

//////////////
// Test Graphs
extern chordalg::AdjacencyLists atom_test;
extern std::vector< chordalg::AdjacencyLists > atom_test_atoms;
extern chordalg::AdjacencyLists bipartite_reduction;
extern chordalg::AdjacencyLists bipartite_reduction_st;
extern chordalg::AdjacencyLists bipartite_reduction_x;
extern chordalg::AdjacencyLists cig_test;
extern chordalg::AdjacencyLists clique;
extern chordalg::AdjacencyLists connected_components_test;
extern chordalg::AdjacencyLists dimacs_test;
extern chordalg::AdjacencyLists nexus_test;
extern chordalg::AdjacencyLists tree;
extern chordalg::AdjacencyLists two_cliques;

#endif  // INCLUDE_TEST_GRAPHS_H_
