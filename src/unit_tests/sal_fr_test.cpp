#include "graph_test.h"

// A simple test on a graph with two disjoint cliques. Intended to test the file reader, sorting / no self-loops of neighborhoods.
TEST_F(SortedAdjacencyListTest, FileReaderTest) {
    Read(graph_dir() + std::string("twocliques.sal"));

    EXPECT_EQ(G->order(),7);
    EXPECT_EQ(G->size(),11);

for(chordalg::Vertex v : *G) {
        chordalg::Vertex w = *(G->begin());
for(chordalg::Vertex u : G->N(v))
        {
            EXPECT_NE(u,v);     // no self-loops
            EXPECT_LE(w,u);     // nbhd sorted
            w = u;

            if(v <= 4) {
                // vertices 0-4 form a clique
                size_t clique_size = 4;
                EXPECT_EQ(G->N(v).size(),clique_size);
                EXPECT_LE(u,4);
            }
            else {
                size_t clique_size = 1;
                // vertices 5 & 6 form a clique
                EXPECT_EQ(G->N(v).size(),clique_size);
                EXPECT_GT(u,4);
            }
        }
    }

    return;
}

