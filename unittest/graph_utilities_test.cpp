#include "graph_test.h"

TEST_F(SortedAdjacencyListTest, GraphUtilities) {
    Read(graph_dir() + std::string("twocliques.sal"));
    EXPECT_EQ(G->order(),7);
    EXPECT_EQ(G->size(),11);

    chordalg::VertexContainer clique, vertices;
    int i = 0;
    for( chordalg::Vertex v : *G )
    {
        vertices.push_back(v);
        if(i < 5)
            clique.push_back(v);
        i++;
    }
    EXPECT_EQ(G->HasClique(clique),true);
    EXPECT_EQ(G->HasClique(vertices),false);

    EXPECT_EQ(G->IsIsomorphic(*G),true);
    return;
}

