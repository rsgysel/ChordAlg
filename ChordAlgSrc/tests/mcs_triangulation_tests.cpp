#include "gtest/gtest.h"

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs.h"
#include "ChordAlgSrc/triangulation.h"
#include "test_graphs.h"

class MCSTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        eo_ = nullptr;
        H_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete eo_;
        delete H_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || eo_ || H_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            eo_ = chordalg::MCS::Run(*G_);
            H_ = chordalg::Triangulation::New(G_, eo_);
        }
    }
 protected:
    chordalg::Graph* G_;
    chordalg::EliminationOrder* eo_;
    chordalg::Triangulation* H_;
};  // MCSTest

//////////////////////
// Triangulation Tests
// These tests compute a triangulation of a graph using MCS,
// then run MCS on the triangulation via IsTriangulated to check
// that it is a chordal graph

TEST_F(MCSTest, BipartiteReductionTriangulation) {
    Read(bipartite_reduction);
    EXPECT_EQ(H_->IsTriangulated(), true);
}

TEST_F(MCSTest, ManyMinsepsFourTriangulation) {
    Read(many_minseps_four);
    EXPECT_EQ(H_->IsTriangulated(), true);
}

TEST_F(MCSTest, TreeTriangulation) {
    Read(tree);
    EXPECT_EQ(H_->IsTriangulated(), true);
}

TEST_F(MCSTest, BipartiteReductionNotPerfect) {
    Read(bipartite_reduction);
    EXPECT_EQ(eo_->IsPerfect(), false);
}

TEST_F(MCSTest, ManyMinsepsFourNotPerfect) {
    Read(many_minseps_four);
    EXPECT_EQ(eo_->IsPerfect(), false);
}

TEST_F(MCSTest, TreeIsPerfect) {
    Read(tree);
    EXPECT_EQ(eo_->IsPerfect(), true);
}
