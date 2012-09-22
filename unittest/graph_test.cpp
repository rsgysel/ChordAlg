#include "graph_test.h"

#ifdef TESTS_ON

typedef chordalg::VertexContainer::const_iterator Nitr;

TEST_F(SortedAdjacencyListFRTest, TwoCliquesData) {
    Read(graph_dir() + std::string("twocliques.sal.txt"));
    EXPECT_EQ(G->order(),7);
    EXPECT_EQ(G->size(),11);

//    for(chordalg::Vertex &v : G->begin() )
//       std::cout << v << " ";
//    std::cout << std::endl;

    for(chordalg::Vertex v = G->begin(); v != G->end(); ++v){

        chordalg::Vertex u = G->begin() - 1;
        for(Nitr i = G->N(v).begin(); i != G->N(v).end(); ++i){
            EXPECT_NE(u,v);     // consistency
            EXPECT_LT(u,*i);    // sorting
            u = *i;

            if(v <= 4){
                // vertices 0-4 form a clique
                EXPECT_EQ(G->N(v).size(),4);
                EXPECT_LE(u,4);
            }
            else{
                // vertices 5 & 6 form a clique
                EXPECT_EQ(G->N(v).size(),1);
                EXPECT_GT(u,4);
            }
        }
    }
}

#endif // TESTS_ON
