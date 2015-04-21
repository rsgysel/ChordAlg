#include "gtest/gtest.h"

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separator_algorithms.h"
#include "ChordAlgSrc/separator_graph.h"
#include "test_graphs.h"

/////////////
// Frameworks

class SeparatorGraphTest : public testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        MS_ = nullptr;
        SepG_ = nullptr;
        A_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete SepG_;
        delete A_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || MS_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            MS_ = chordalg::MinimalSeparators(*G_);
            SepG_ = new chordalg::SeparatorGraph(G_, MS_);
            A_ = new chordalg::Atoms(G_);
            A_->ComputeAtoms();
        }
        return;
    }
    void CompareCrossingRelations() {
        size_t size = 0;
        for (auto H : *A_) {
            chordalg::MinsepTrie* H_MS = chordalg::MinimalSeparators(*H);
            chordalg::SeparatorGraph SepH(H, H_MS);
            size += SepH.size();
        }
        EXPECT_EQ(size, SepG_->size());
    }
 protected:
    chordalg::Graph* G_;
    chordalg::MinsepTrie* MS_;
    chordalg::SeparatorGraph* SepG_;
    chordalg::Atoms* A_;
};  // SeparatorGraphTest

////////
// Tests

TEST_F(SeparatorGraphTest, TreeXRel) {
    Read(tree);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, BipartiteReductionXRel) {
    Read(bipartite_reduction);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, xBipartiteReductionXRel) {
    Read(bipartite_reduction_x);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, stBipartiteReductionXRel) {
    Read(bipartite_reduction_st);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, DimacsXRel) {
    Read(dimacs_test);
    CompareCrossingRelations();
}

// This integration test is brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// the order in which the minimal separators are read
// by the SeparatorGraph constructor

TEST_F(SeparatorGraphTest, CliqueAutomorphism) {
    Read(clique);
    for (auto H : *A_) {
        chordalg::MinsepTrie* H_MS = chordalg::MinimalSeparators(*H);
        chordalg::SeparatorGraph SepH(H, H_MS);
        EXPECT_EQ(SepH.IsIsomorphic(*SepG_), true);
    }
}
