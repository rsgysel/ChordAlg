#include "elimination_ordering.h"
#include "graph_test.h"

TEST_F(SortedAdjacencyListTest, EliminationOrderStar) {
    Read(graph_dir() + std::string("star.sal"));
    chordalg::EliminationOrdering pi(*G);
    EXPECT_EQ(pi.fill_size(),15);
    pi.Swap(0,4);       EXPECT_EQ(pi.fill_size(),1);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(4,0);       EXPECT_EQ(pi.fill_size(),15);   EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(0,6);       EXPECT_EQ(pi.fill_size(),0);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(6,0);       EXPECT_EQ(pi.fill_size(),15);   EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(0,3);       EXPECT_EQ(pi.fill_size(),3);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(3,4);       EXPECT_EQ(pi.fill_size(),1);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(4,5);       EXPECT_EQ(pi.fill_size(),0);    EXPECT_EQ(pi.IsOrderConsistent(),true);

    pi.MoveAndPush(2,3);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(2,5);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(4,6);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(1,3);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(0,2);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(1,5);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.MoveAndPush(4,5);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    return;
}

TEST_F(SortedAdjacencyListTest, EliminationOrderTree) {
    Read(graph_dir() + std::string("tree.sal"));
    chordalg::EliminationOrdering pi(*G);
    EXPECT_EQ(pi.fill_size(),0);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    pi.Swap(0,13);
    EXPECT_EQ(pi.fill_size(),4);    EXPECT_EQ(pi.IsOrderConsistent(),true);
    return;
}
