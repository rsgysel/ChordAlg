#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lb_triang.h"
#include "ChordAlgSrc/triangulation.h"
#include "test_graphs.h"

class LBTriangTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        H_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete H_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || H_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            H_ = chordalg::LBTriang::Run(G_);
            EXPECT_EQ(H_->IsMinimalTriangulation(), true);
        }
    }
 protected:
    chordalg::Graph* G_;
    chordalg::Triangulation* H_;
};  // LBTriangTest

//////////////////////////////
// Minimal Triangulation Tests
// LBTriang should always return a minimal triangulation

TEST_F(LBTriangTest, AtomTestMinimalTriangulation) {
    Read(atom_test);
}

TEST_F(LBTriangTest, BipartiteReductionMinimalTriangulation) {
    Read(bipartite_reduction);
}

TEST_F(LBTriangTest, DimacsTestMinimalTriangulation) {
    Read(dimacs_test);
}

TEST_F(LBTriangTest, IndependentSetMinimalTriangulation) {
    Read(independent_set);
}

TEST_F(LBTriangTest, ManyMinsepsFourMinimalTriangulation) {
    Read(many_minseps_four);
}

TEST_F(LBTriangTest, TreeMinimalTriangulation) {
    Read(tree);
}
