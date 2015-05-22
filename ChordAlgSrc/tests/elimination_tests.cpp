#include "gtest/gtest.h"

#include "ChordAlgSrc/elimination.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

class EliminationTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        eo_ = nullptr;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || eo_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            eo_ = new chordalg::EliminationOrder(G_);
        }
    }
    void TearDown() {
        delete G_;
        delete eo_;
    }
 protected:
    chordalg::Graph* G_;
    chordalg::EliminationOrder* eo_;
};  // EliminationTest

class EliminationConnectedTest : public EliminationTest {
 public:
    void SetUp() {
        G_ = nullptr;
        eo_ = nullptr;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || eo_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            eo_ = chordalg::Elimination::ConnectedOrder(*G_);
            chordalg::Vertices V;
            // Check that every G[v_1, v_2, ..., v_i] is connected
            for (size_t i = 0; i < G_->order(); ++i) {
                V.push_back(eo_->VertexAt(i));
                chordalg::InducedSubgraph* GV = chordalg::InducedSubgraph::New(G_, V);
                chordalg::SeparatorComponents S(GV);
                S.Separate(chordalg::Vertices());
                EXPECT_EQ(S.size(), 1);
                delete GV;
            }
        }
    }
    void TearDown() {
        delete G_;
        delete eo_;
    }
 protected:
    chordalg::Graph* G_;
    chordalg::EliminationOrder* eo_;
};  // EliminationConnectedTest

// Every elimination order is perfect for these graphs

TEST_F(EliminationTest, IndependentSetIsPerfect) {
    Read(independent_set);
    EXPECT_EQ(chordalg::Elimination::ZeroFill(*G_, *eo_), true);
}

TEST_F(EliminationTest, TwoCliquesIsPerfect) {
    Read(two_cliques);
    EXPECT_EQ(chordalg::Elimination::ZeroFill(*G_, *eo_), true);
}

// Each of these graphs is connected

TEST_F(EliminationConnectedTest, BipartiteReduction) {
    Read(bipartite_reduction);
}

TEST_F(EliminationConnectedTest, BipartiteReductionST) {
    Read(bipartite_reduction_st);
}

TEST_F(EliminationConnectedTest, BipartiteReductionX) {
    Read(bipartite_reduction_x);
}

TEST_F(EliminationConnectedTest, Clique) {
    Read(clique);
}

TEST_F(EliminationConnectedTest, ManyMinsepsFour) {
    Read(many_minseps_four);
}

TEST_F(EliminationConnectedTest, Tree) {
    Read(tree);
}
