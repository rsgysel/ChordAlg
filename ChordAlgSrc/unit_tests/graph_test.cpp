#include <utility>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertex_utilities.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

////////////////////
// Mock Dependencies

class MockFileReader : public chordalg::FileReader {
 public:
    MockFileReader() = delete;
    ~MockFileReader() {}
    MOCK_METHOD0(TakeNeighborhoods, chordalg::AdjacencyLists*());
    MOCK_METHOD0(TakeNames, chordalg::VertexNames*());
};  // MockFileReader

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
    GraphTest() : G_(nullptr) {};
    void SetUp() {
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
    for(chordalg::Vertex v : *G_) {
        for(chordalg::Vertex u : G_->N(v)) {
            size_t clique_size = 0;
            if(v <= 4) { // Vertices 0-4 form a clique
                clique_size = 5;
                EXPECT_LE(u, 4);
            } else {
                clique_size = 2;
                EXPECT_GT(u, 4);
            }
            EXPECT_EQ(G_->N(v).size(), clique_size - 1);
        }
    }
}

