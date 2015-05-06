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
        Geo_ = nullptr;
        H1_ = nullptr;
        Heo_ = nullptr;
        H2_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete Geo_;
        delete H1_;
        delete Heo_;
        delete H2_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || Geo_ || H1_ || Heo_ || H2_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            Geo_ = MCS(*G_);
std::cout << __LINE__ << ":" << __PRETTY_FUNCTION__;
std::cout << Geo_->ComputeFill()->fill_count() << "\n";
            H1_ = chordalg::Triangulation::New(G_, Geo_);
std::cout << __LINE__ << ":" << __PRETTY_FUNCTION__;
std::cout << G_->size() << " " << H1_->size() << "\n";
            Heo_ = MCS(*H1_);
            H2_ = chordalg::Triangulation::New(H1_, Heo_);
        }
    }
 protected:
    chordalg::Graph* G_;
    chordalg::EliminationOrder* Geo_;
    chordalg::Triangulation* H1_;
    chordalg::EliminationOrder* Heo_;
    chordalg::Triangulation* H2_;
};  // MCSTest

//////////////////////
// Triangulation Tests
// These tests compute a triangulation of a graph using MCS,
// then run MCS on the triangulation. The resulting graph should
// be the same.

TEST_F(MCSTest, BipartiteReductionTriangulation) {
    Read(bipartite_reduction);
    EXPECT_EQ(H1_->IsIsomorphic(*H2_), true);
}

TEST_F(MCSTest, ManyMinsepsFourTriangulation) {
    Read(many_minseps_four);
    EXPECT_EQ(H1_->IsIsomorphic(*H2_), true);
}

TEST_F(MCSTest, TreeTriangulation) {
    Read(tree);
    EXPECT_EQ(H1_->IsIsomorphic(*H2_), true);
}

TEST_F(MCSTest, BipartiteReductionIsPerfect) {
    Read(bipartite_reduction);
    EXPECT_EQ(Heo_->IsPerfect(), true);
}

TEST_F(MCSTest, ManyMinsepsFourIsPerfect) {
    Read(many_minseps_four);
    EXPECT_EQ(Heo_->IsPerfect(), true);
}

TEST_F(MCSTest, TreeIsPerfect) {
    Read(tree);
    EXPECT_EQ(Heo_->IsPerfect(), true);
}
