#include "gtest/gtest.h"

#include "atoms_test_framework.h"
#include "test_graphs.h"

// The integration tests in this file are brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// how the file reader parses a graph file.

TEST_F(AtomsTest, Integration) {
    Read(atom_test);
    size_t i = 0;
    for(auto a : *A_) {
        chordalg::Graph G(new chordalg::AdjacencyLists(atom_test_atoms[i]));
        EXPECT_EQ(a->IsIsomorphic(G), true);
        ++i;
    }
}

