#include "gtest/gtest.h"

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "graph_test_framework.h"
#include "test_graphs.h"

class AtomsTest : public GraphTest {
 public:
    void SetUp() {
        G_ = nullptr;
        A_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete A_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || A_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            A_ = new chordalg::Atoms(G_);
            A_->ComputeAtoms();
        }
        return;
    }
 protected:
    chordalg::Atoms* A_;
};  // AtomsTest


// The integration tests in this file are brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// how the file reader parses a graph file.

TEST_F(AtomsTest, PaperFigureIsomorphism) {
    Read(atom_test);
    size_t i = 0;
    for (auto a : *A_) {
        chordalg::Graph G(new chordalg::AdjacencyLists(atom_test_atoms[i]));
        EXPECT_EQ(a->IsIsomorphic(G), true);
        ++i;
    }
}

TEST_F(AtomsTest, IndependentSetIsomorphism) {
    Read(independent_set);
    for (auto a : *A_) {
        EXPECT_EQ(a->order(), 1);
    }
}

TEST_F(AtomsTest, PaperFigureSize) {
    Read(atom_test);
    EXPECT_EQ(A_->size(), 4);
}

TEST_F(AtomsTest, CliquesSize) {
    Read(two_cliques);
    EXPECT_EQ(A_->size(), 2);
}

TEST_F(AtomsTest, IndependentSetSize) {
    Read(independent_set);
    EXPECT_EQ(A_->size(), G_->order());
}
