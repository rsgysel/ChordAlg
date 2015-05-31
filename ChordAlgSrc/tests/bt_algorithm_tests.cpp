#include "gtest/gtest.h"

#include "ChordAlgSrc/bt_algorithm.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

/////////////
// Frameworks

class FullBlocksTest : public testing ::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        MS_ = nullptr;
        B_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete MS_;
        delete B_;
    }
    void Read(const chordalg::AdjacencyLists& A, size_t full_block_count, size_t inclusion_minimal_full_block_count) {
        if (G_ || MS_ || B_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            MS_ = chordalg::MinimalSeparators::Generate(*G_);
            B_ = chordalg::BTAlgorithm::GoodTripleBuckets::New(G_, *MS_);
            size_t full_blocks_seen = 0, inclusion_minimal_full_blocks_seen = 0;
            for (auto bucket : B_->good_triples()) {
                for (auto pair : bucket) {
                    ++full_blocks_seen;
                    if (G_->IsClique(pair.first.C())) {
                        ++inclusion_minimal_full_blocks_seen;
                    }
                }
            }
            EXPECT_EQ(full_block_count, full_blocks_seen);
            EXPECT_EQ(inclusion_minimal_full_block_count, inclusion_minimal_full_blocks_seen);
        }
    }

 protected:
    chordalg::Graph* G_;
    chordalg::MinsepTrie* MS_;
    chordalg::BTAlgorithm::GoodTripleBuckets* B_;
};  // FullBlocksTest

////////
// Tests

TEST_F(FullBlocksTest, Tree) {
    Read(tree, 18, 8);
}

TEST_F(FullBlocksTest, ManyMinsepsFour) {
    Read(many_minseps_four, 52, 22);
}
