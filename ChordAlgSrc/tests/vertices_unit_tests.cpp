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
        for(size_t i = 0; i < 6; ++i) {
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
    for(auto pair : chordalg::VertexPairs(V_)) {
        EXPECT_NE(pair.first, pair.second);
        ++pairs_count;
    }
    EXPECT_EQ(pairs_count, 15);
}

TEST_F(VertexPairsTest, Sorted) {
    for(auto pair : chordalg::VertexPairs(V_)) {
        EXPECT_LE(0, pair.first);
        EXPECT_LT(pair.first, pair.second);
        EXPECT_LT(pair.second, 6);
    } 
}

