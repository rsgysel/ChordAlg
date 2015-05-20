#include "gtest/gtest.h"

#include <vector>

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

typedef std::vector< chordalg::VertexPair > VertexPairVector;

class TriangulationTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        F_ = nullptr;
        H_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete F_;
        delete H_;
    }
    void Read(chordalg::AdjacencyLists& A,
              VertexPairVector fill = VertexPairVector()) {
        if (G_ || F_ || H_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            F_ = new chordalg::FillEdges(G_);
            for (chordalg::VertexPair uv : fill) {
                F_->AddEdge(uv);
            }
            H_ = chordalg::Triangulation::New(G_, *F_);
        }
    }
 protected:
    chordalg::Graph* G_;
    chordalg::FillEdges* F_;
    chordalg::Triangulation* H_;
};  // TriangulationTest

// The following graphs are not chordal, so are not minimal triangulations

TEST_F(TriangulationTest, BipartiteReductionNotTriangulated) {
    Read(bipartite_reduction);
    EXPECT_EQ(H_->IsMinimalTriangulation(), false);
}

TEST_F(TriangulationTest, ManyMinsepsFourNotTriangulated) {
    Read(many_minseps_four);
    EXPECT_EQ(H_->IsMinimalTriangulation(), false);
}

// The following graphs are chordal, so they are minimal triangulations of themselves

TEST_F(TriangulationTest, IndependentSetSelfMinimalTriangulation) {
    Read(independent_set);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

TEST_F(TriangulationTest, TreeSelfMinimalTriangulation) {
    Read(tree);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

TEST_F(TriangulationTest, TwoCliquesSelfMinimalTriangulation) {
    Read(two_cliques);
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

// Fill edges are added to create non-minimal triangulations

TEST_F(TriangulationTest, IndependentSetNonminimalTriangulation) {
    Read(independent_set, {{0, 1}, {1, 2}, {2, 3}});
    EXPECT_EQ(H_->IsTriangulated(), true);
    EXPECT_EQ(H_->IsMinimalTriangulation(), false);
}

TEST_F(TriangulationTest, TwoCliquesNonminimalTriangulation) {
    Read(two_cliques, {{0, 5}, {0, 6}});
    EXPECT_EQ(H_->IsTriangulated(), true);
    EXPECT_EQ(H_->IsMinimalTriangulation(), false);
}

// Fill edges are added to create minimal triangulations
// These tests are brittle: they depend on test_graphs.cpp

TEST_F(TriangulationTest, ManyMinsepsForMinimalTriangulation) {
    Read(many_minseps_four, {{0, 1}, {0, 6}, {0, 7}, {0, 8}, {0, 9}});
    EXPECT_EQ(H_->IsMinimalTriangulation(), true);
}

