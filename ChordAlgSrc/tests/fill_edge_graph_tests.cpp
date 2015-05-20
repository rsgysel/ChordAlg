#include "gtest/gtest.h"

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

class FillEdgesTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        F_ = nullptr;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || F_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            F_ = new chordalg::FillEdges(G_);
            TestFillFunction();
            EXPECT_EQ(2 * (F_->fill_count() + G_->size()),
                      G_->order() * (G_->order() - 1));
        }
    }
    virtual void TestFillFunction() = 0;
    void TearDown() {
        delete G_;
        delete F_;
    }
 protected:
    chordalg::Graph* G_;
    chordalg::FillEdges* F_;
};  // FillEdgesTest

class AddEdgeTest : public FillEdgesTest {
 public:
    void TestFillFunction() {
        for (chordalg::VertexPair uv : chordalg::VertexPairs(G_->V())) {
            F_->AddEdge(uv);
        }
    }
};  // AddEdgeTest

class SaturateTest : public FillEdgesTest {
 public:
    void TestFillFunction() {
        chordalg::Vertices V = G_->V();
        F_->Saturate(V);
    }
};  // SaturateTest

///////////////
// AddEdgeTests

TEST_F(AddEdgeTest, BipartiteReduction) {
    Read(bipartite_reduction);
}

TEST_F(AddEdgeTest, IndependentSet) {
    Read(independent_set);
}

TEST_F(AddEdgeTest, TwoCliques) {
    Read(two_cliques);
}

////////////////
// SaturateTests

TEST_F(SaturateTest, BipartiteReduction) {
    Read(bipartite_reduction);
}

TEST_F(SaturateTest, IndependentSet) {
    Read(independent_set);
}

TEST_F(SaturateTest, TwoCliques) {
    Read(two_cliques);
}

