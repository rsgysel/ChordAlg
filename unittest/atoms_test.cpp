#include "atoms.h"
#include "graph_test.h"

// A simple test on a graph with two disjoint cliques. Intended to test the file reader, sorting / no self-loops of neighborhoods.
TEST_F(SortedAdjacencyListTest, AtomsOfTwoCliques) {
    Read(graph_dir() + std::string("twocliques.sal"));
    chordalg::Atoms A(*G);

    bool first_clique = true;
    for( chordalg::AtomSubgraph* a : A )
    {
        if(first_clique)
        {
            EXPECT_EQ(a->order(),2);
            EXPECT_EQ(a->size(),1);
            first_clique = false;
        }
        else{
            EXPECT_EQ(a->order(),5);
            EXPECT_EQ(a->size(),10);
        }
    }
    return;
}

TEST_F(SortedAdjacencyListTest, AtomsOfPaperEx) {
    Read(graph_dir() + std::string("atom_test.sal"));
    chordalg::Atoms A(*G);

    for( chordalg::AtomSubgraph* a : A )
    {
        a->PrettyPrint();
    }
    return;
}
