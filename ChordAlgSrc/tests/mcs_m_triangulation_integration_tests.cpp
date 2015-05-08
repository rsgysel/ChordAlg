#include "gtest/gtest.h"

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs_m.h"
#include "ChordAlgSrc/triangulation.h"
#include "test_graphs.h"

class MCSmPlusTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        eo_ = nullptr;
        F_ = nullptr;
        H_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete eo_;
        delete F_;
        delete H_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || eo_ || F_ || H_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            eo_ = new chordalg::EliminationOrder(G_);
            F_ = new chordalg::FillEdges(G_);
            MCSmPlus(*G_, eo_, F_);
            H_ = chordalg::Triangulation::New(G_, eo_);
        }
    }
 protected:
    chordalg::Graph* G_;
    chordalg::EliminationOrder* eo_;
    chordalg::FillEdges* F_;
    chordalg::Triangulation* H_;
};  // MCSmPlusTest

//////////////////////////////
// Minimal Triangulation Tests
// MCSmPlus should always return a minimal triangulation

TEST_F(MCSmPlusTest, BipartiteReductionTriangulation) {
    Read(bipartite_reduction);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

TEST_F(MCSmPlusTest, ManyMinsepsFourTriangulation) {
    Read(many_minseps_four);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

TEST_F(MCSmPlusTest, TreeTriangulation) {
    Read(tree);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

