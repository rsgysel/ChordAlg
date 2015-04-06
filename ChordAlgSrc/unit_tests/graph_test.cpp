#include <utility>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertex_utilities.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

//////////////
// Test Graphs
chordalg::AdjacencyLists two_cliques{ {1, 2, 3, 4}, 
                                      {0, 2, 3, 4},
                                      {0, 1, 3, 4},
                                      {0, 1, 2, 4},
                                      {0, 1, 2, 3},
                                      {6},
                                      {5} };

/////////////
// Frameworks

class GraphTest : public ::testing::Test {
  public:
    void SetUp() {
        G_ = nullptr;
    }
    void TearDown() {
        delete G_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if(G_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
        }
        return;
    }
  protected:
    chordalg::Graph* G_;
};  // GraphTest

TEST_F(GraphTest, Order) {
    Read(two_cliques);
    EXPECT_EQ(G_->order(), 7); 
}

TEST_F(GraphTest, Size) {
    Read(two_cliques);
    EXPECT_EQ(G_->size(), 11); 
}

TEST_F(GraphTest, NbhdValueRange) {
    Read(two_cliques);
    for(chordalg::Vertex v : *G_) {
        for(chordalg::Vertex u : G_->N(v)) {
            EXPECT_GE(u, 0);
            EXPECT_LT(u, G_->order());
        }
    }
}

TEST_F(GraphTest, NoSelfLoops) {
    Read(two_cliques);
    for(chordalg::Vertex v : *G_) {
        for(chordalg::Vertex u : G_->N(v)) {
            EXPECT_NE(u, v);
        }
    }
}

TEST_F(GraphTest, SortedNeighborhoods) {
    Read(two_cliques);
    for(chordalg::Vertex v : *G_) {
        chordalg::Vertex w = *(G_->begin());
        for(chordalg::Vertex u : G_->N(v)) {
            EXPECT_LE(w, u);
        }
    }
}

TEST_F(GraphTest, Cliques) {
    Read(two_cliques);
    chordalg::VertexVector K1{0, 1, 2, 3, 4}, K2{5, 6};
    EXPECT_EQ(G_->HasClique(K1), true);
    EXPECT_EQ(G_->HasClique(K2), true);
}

TEST_F(GraphTest, Automorphism) {
    Read(two_cliques);
    EXPECT_EQ(G_->IsIsomorphic(*G_), true);
}
