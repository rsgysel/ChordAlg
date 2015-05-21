#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/vertices.h"

using ::testing::_;
using ::testing::ReturnRef;

/////////////
// Frameworks

class VertexPairsTest : public ::testing::Test {
 public:
    VertexPairsTest() : V_(6) {}
    void SetUp() {
        for (size_t i = 0; i < 6; ++i) {
            V_[i] = i;
        }
    }
    void TearDown() {}
 protected:
    chordalg::Vertices V_;
};  // VertexPairsTest

////////
// Tests

TEST_F(VertexPairsTest, Count) {
    size_t pairs_count = 0;
    for (auto pair : chordalg::VertexPairs(V_)) {
        EXPECT_NE(pair.first, pair.second);
        ++pairs_count;
    }
    EXPECT_EQ(pairs_count, 15);
}

TEST_F(VertexPairsTest, Sorted) {
    for (auto pair : chordalg::VertexPairs(V_)) {
        EXPECT_LE(0, pair.first);
        EXPECT_LT(pair.first, pair.second);
        EXPECT_LT(pair.second, 6);
    } 
}

TEST(VertexAlgorithmsTest, SetUnion) {
    chordalg::Vertices U{0, 1, 3}, W{1, 2, 4};
    chordalg::Vertices V;
    V.SetUnion(U, W);
    EXPECT_EQ(V.size(), 5);
}

TEST(VertexAlgorithmsTest, SetUnionOneEmpty) {
    chordalg::Vertices U, W{1, 2};
    chordalg::Vertices V;
    V.SetUnion(U, W);
    EXPECT_EQ(V.size(), 2);
    V.SetUnion(W, U);
    EXPECT_EQ(V.size(), 2);
}

TEST(VertexAlgorithmsTest, SetUnionBothEmpty) {
    chordalg::Vertices U, W;
    chordalg::Vertices V;
    V.SetUnion(U, W);
    EXPECT_EQ(V.size(), 0);
}

TEST(VertexAlgorithmsTest, SetIntersection) {
    chordalg::Vertices U{0, 1, 3}, W{1, 2, 4};
    chordalg::Vertices V;
    V.SetIntersection(U, W);
    EXPECT_EQ(V.size(), 1);
}

TEST(VertexAlgorithmsTest, SetIntersectionOneEmpty) {
    chordalg::Vertices U, W{1, 2};
    chordalg::Vertices V;
    V.SetIntersection(U, W);
    EXPECT_EQ(V.size(), 0);
    V.SetIntersection(W, U);
    EXPECT_EQ(V.size(), 0);
}

TEST(VertexAlgorithmsTest, SetIntersectionBothEmpty) {
    chordalg::Vertices U, W;
    chordalg::Vertices V;
    V.SetIntersection(U, W);
    EXPECT_EQ(V.size(), 0);
}


