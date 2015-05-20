#include "gtest/gtest.h"

#include "ChordAlgSrc/elimination.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "test_graphs.h"

class EliminationOrderTest : public ::testing::Test {
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
};  // EliminationOrderTest


// Every elimination order is perfect for these graphs

TEST_F(EliminationOrderTest, IndependentSetIsPerfect) {
    Read(independent_set);
    EXPECT_EQ(chordalg::Elimination::ZeroFill(*G_, *eo_), true);
}

TEST_F(EliminationOrderTest, TwoCliquesIsPerfect) {
    Read(two_cliques);
    EXPECT_EQ(chordalg::Elimination::ZeroFill(*G_, *eo_), true);
}
