#include <utility>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"
#include "graph_test_framework.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

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
    for (chordalg::Vertex v : *G_) {
        for (chordalg::Vertex u : G_->N(v)) {
            EXPECT_GE(u, 0);
            EXPECT_LT(u, G_->order());
        }
    }
}

TEST_F(GraphTest, NoSelfLoops) {
    Read(two_cliques);
    for (chordalg::Vertex v : *G_) {
        for (chordalg::Vertex u : G_->N(v)) {
            EXPECT_NE(u, v);
        }
    }
}

TEST_F(GraphTest, SortedNeighborhoods) {
    Read(two_cliques);
    for (chordalg::Vertex v : *G_) {
        chordalg::Vertex w = *(G_->begin());
        for (chordalg::Vertex u : G_->N(v)) {
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
