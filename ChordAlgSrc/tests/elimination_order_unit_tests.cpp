#include "gmock/gmock.h"
#include "gtest/gtest.h"

////////
// Mocks

class MockGraph : public chordalg::Graph {
 public:
    MockGraph() {
        ON_CALL(this, order())
            .WillAlways(Return(4));
    }
    ~MockGraph() {}
    MOCK_METHOD0(order, size_t());
};  // MockGraph

/////////////
// Frameworks


TEST(EliminationOrder, Size) {
    MockGraph G;
    EliminationOrder eo(&G);
    EXPECT_EQ(eo.size(), 4);
}
