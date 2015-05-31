#include <typeinfo>

#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "test_graphs.h"

/////////////
// Frameworks

template< class SeparatorType >
class SeparatorComponentsTest : public ::testing::Test {
  public:
    void SetUp() {
        G_ = nullptr;
        S_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete S_;
    }
    void Read(const chordalg::AdjacencyLists& A, const chordalg::Vertices& X) {
        if (G_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            S_ = new SeparatorType(G_);
            S_->Separate(X);
        }
    }
    bool TypeIs(const std::type_info& RHS_Type) const {
        return typeid(SeparatorType) == RHS_Type;
    }
  protected:
    chordalg::Graph* G_;
    SeparatorType* S_;
};  // SeparatorComponentsTest

/////////////////////
// DeclaredTypedTests

typedef ::testing::Types< chordalg::SeparatorComponents,
                          chordalg::SeparatorBlocks >
                          SeparatorTypes;
TYPED_TEST_CASE(SeparatorComponentsTest, SeparatorTypes);

//////////////
// Typed Tests

TYPED_TEST(SeparatorComponentsTest, Connected) {
    this->Read(connected_components_test, chordalg::Vertices());
    EXPECT_EQ(this->S_->size(), 1);
}

TYPED_TEST(SeparatorComponentsTest, Size) {
    this->Read(connected_components_test, chordalg::Vertices({0, 1, 2, 3}));
    EXPECT_EQ(this->S_->size(), 4);
}

TYPED_TEST(SeparatorComponentsTest, Searched) {
    this->Read(connected_components_test, chordalg::Vertices());
    for (auto v : *(this->G_)) {
        EXPECT_NE(this->S_->ComponentId(v), this->S_->kUnsearched());
    }
}

// Compares ComponentId of adjacent vertices not in the separator
TYPED_TEST(SeparatorComponentsTest, NeighborsConnected) {
    this->Read(connected_components_test, chordalg::Vertices({0, 1, 2, 3}));
    EXPECT_EQ(this->S_->size(), 4);
    for (auto v : *(this->G_)) {
        if (!this->S_->IsInSeparator(v)) {
            for (auto u : this->G_->N(v)) {
                if (!this->S_->IsInSeparator(u)) {
                    EXPECT_EQ(this->S_->ComponentId(u), this->S_->ComponentId(v));
                }
            }
        }
    }
}

////////
// Tests

TEST(SeparatorBlocksTest, EmptySeparatorNeighborhoodSize) {
    chordalg::Graph G(new chordalg::AdjacencyLists(connected_components_test));
    chordalg::SeparatorBlocks S(&G);
    S.Separate(chordalg::Vertices());
    for (auto B : S) {
        EXPECT_EQ(B.NC().size(), 0);
    }
}

TEST(SeparatorBlocksTest, NeighborhoodSizes) {
    chordalg::Graph G(new chordalg::AdjacencyLists(connected_components_test));
    chordalg::SeparatorBlocks S(&G);
    S.Separate(chordalg::Vertices({0, 1, 2, 3}));
    size_t i = 0, neighborhood_sizes[]{4, 1, 2, 4};
    for (auto B : S) {
        EXPECT_EQ(B.NC().size(), neighborhood_sizes[i]);
        ++i;
    }
}

TEST(SeparatorBlocksTest, IsInclusionMinimal) {
    chordalg::Graph G(new chordalg::AdjacencyLists(connected_components_test));
    chordalg::SeparatorBlocks S(&G);
    S.Separate(chordalg::Vertices({2, 3}));
    EXPECT_EQ(S.IsInclusionMinimal(), true);
}

TEST(SeparatorBlocksTest, NotInclusionMinimal) {
    chordalg::Graph G(new chordalg::AdjacencyLists(connected_components_test));
    chordalg::SeparatorBlocks S(&G);
    S.Separate(chordalg::Vertices({0, 1, 2, 3}));
    EXPECT_EQ(S.IsInclusionMinimal(), false);
}
