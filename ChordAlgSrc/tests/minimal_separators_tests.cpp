#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

/////////////
// Frameworks

class MinimalSeparatorAlgorithmsTest : public testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        MS_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete MS_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || MS_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            MS_ = chordalg::MinimalSeparators::Generate(G_);
        }
        return;
    }
    void Read(chordalg::AdjacencyLists& A, chordalg::Vertex u, chordalg::Vertex v) {
        if (G_ || MS_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            MS_ = chordalg::MinimalSeparators::Generate(G_, u, v);
        }
        return;
    }
 protected:
    chordalg::Graph* G_;
    chordalg::MinsepTrie* MS_;
};  // MinimalSeparatorAlgorithmsTest

////////
// Tests

// Nothing separates a clique
TEST_F(MinimalSeparatorAlgorithmsTest, Clique) {
    Read(clique);
    EXPECT_EQ(MS_->size(), 0);
}

// TwoCliques consists of two disjoint cliques; empty set is a separator
TEST_F(MinimalSeparatorAlgorithmsTest, TwoCliques) {
    Read(two_cliques);
    EXPECT_EQ(MS_->size(), 1);
}

// One-to-one correspondance between internal nodes and minimal separators
TEST_F(MinimalSeparatorAlgorithmsTest, Tree) {
    Read(tree);
    EXPECT_EQ(MS_->size(), 6);
}

TEST_F(MinimalSeparatorAlgorithmsTest, BipartiteReduction) {
    Read(bipartite_reduction);
    EXPECT_EQ(MS_->size(), 9);
}

TEST_F(MinimalSeparatorAlgorithmsTest, BipartiteReductionPair) {
    Read(bipartite_reduction, 6, 7);
    EXPECT_EQ(MS_->size(), 4);
}

TEST_F(MinimalSeparatorAlgorithmsTest, BipartiteReductionPairBlocks) {
    Read(bipartite_reduction, 6, 7);
    for (auto X : *MS_) {
        chordalg::SeparatorBlocks S(G_);
        chordalg::Vertices V(X);
        S.Separate(&V);
        EXPECT_EQ(S.FullComponentCt(), 2);
        EXPECT_EQ(S.NonFullComponentCt(), 0);
    }
}

TEST_F(MinimalSeparatorAlgorithmsTest, xBipartiteReduction) {
    Read(bipartite_reduction_x, 6, 7);
    EXPECT_EQ(MS_->size(), 4);
}

TEST_F(MinimalSeparatorAlgorithmsTest, xBipartiteReductionBlocks) {
    Read(bipartite_reduction_x, 6, 7);
    for (auto X : *MS_) {
        chordalg::SeparatorBlocks S(G_);
        chordalg::Vertices V(X);
        S.Separate(&V);
        EXPECT_EQ(S.FullComponentCt(), 2);
        EXPECT_EQ(S.NonFullComponentCt(), 0);
    }
}

TEST_F(MinimalSeparatorAlgorithmsTest, stBipartiteReduction) {
    Read(bipartite_reduction_st);
    EXPECT_EQ(MS_->size(), 8);
}

TEST_F(MinimalSeparatorAlgorithmsTest, stBipartiteReductionPair) {
    Read(bipartite_reduction_st, 6, 7);
    EXPECT_EQ(MS_->size(), 0);
}

TEST_F(MinimalSeparatorAlgorithmsTest, stBipartiteReductionBlocks) {
    Read(bipartite_reduction_st, 6, 7);
    for (auto X : *MS_) {
        chordalg::SeparatorBlocks S(G_);
        chordalg::Vertices V(X);
        S.Separate(&V);
        EXPECT_EQ(S.FullComponentCt(), 2);
        EXPECT_EQ(S.NonFullComponentCt(), 0);
    }
}

TEST_F(MinimalSeparatorAlgorithmsTest, AllPairsSanityCheck) {
    Read(many_minseps_four);
    chordalg::LexTrie AllPairsMinseps(G_->order());
    for (auto uv : chordalg::VertexPairs(G_->V()) ) {
        chordalg::LexTrie* PairMinseps = chordalg::MinimalSeparators::Generate(G_, uv.first, uv.second);
        for (auto S : *PairMinseps) {
            AllPairsMinseps.Insert(&S);
        }
        delete PairMinseps;
    }
    EXPECT_EQ(AllPairsMinseps.size(), MS_->size());
}
