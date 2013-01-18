#include "graph_test.h"
#include "connectedcomponentcalculator.h"

// Tests connected component calculation
TEST_F(SortedAdjacencyListTest, ConnectedComponentCalculator) {
    Read(graph_dir() + std::string("cc_test.sal"));
    EXPECT_EQ(G->order(),15);
    EXPECT_EQ(G->size(),23);

    chordalg::ConnectedComponentCalculator CC(*G);
    chordalg::VertexContainer S = {0,1,2,3};
    CC.Calculate(S);
    EXPECT_EQ(CC.size(),4);

    // now check connected component consistency. subroutine below is independent of cc_test.sal
    for(chordalg::Vertex v : *G)
    {
        if(CC[v] != CC.kInSeparator())
        {
            for(chordalg::Vertex u : G->N(v))
            {
                if(CC[u] != CC.kInSeparator())
                    EXPECT_EQ(CC[v],CC[u]);
            } // neighbors not in S are in same connected component
        } // for vertices not in S
    }

    chordalg::ConnectedComponentNeighborhoodCalculator NC(*G,CC);
    NC.Calculate();
    EXPECT_EQ(NC[0].size(),4);
    EXPECT_EQ(NC[1].size(),1);
    EXPECT_EQ(NC[2].size(),2);
    EXPECT_EQ(NC[3].size(),4);

    return;
}
