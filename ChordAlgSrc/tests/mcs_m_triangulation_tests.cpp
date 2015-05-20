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
            chordalg::MCSmPlus::Run(*G_, eo_, F_);
            H_ = chordalg::Triangulation::New(G_, *eo_);
            EXPECT_EQ(H_->IsMinimalTriangulation(), true);
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

TEST_F(MCSmPlusTest, AtomTestMinimalTriangulation) {
    Read(atom_test);
}

TEST_F(MCSmPlusTest, BipartiteReductionMinimalTriangulation) {
    Read(bipartite_reduction);
}

TEST_F(MCSmPlusTest, DimacsTestMinimalTriangulation) {
    Read(dimacs_test);
}

TEST_F(MCSmPlusTest, IndependentSetMinimalTriangulation) {
    Read(independent_set);
}

TEST_F(MCSmPlusTest, ManyMinsepsFourMinimalTriangulation) {
    Read(many_minseps_four);
}

TEST_F(MCSmPlusTest, TreeMinimalTriangulation) {
    Read(tree);
}

