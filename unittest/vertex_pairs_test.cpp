#include "graph.h"
#include "file_reader.h"
#include "graph_test.h"
#include "utilities.hpp"

TEST_F( SortedAdjacencyListTest, Debug )
{
    using namespace chordalg;
    Read(graph_dir() + std::string("atom_test_1.sal"));

    int pairs_count = 0;
    for( int i = 0; i < 6; ++i )
    {
        for( VertexPair p : VertexPairs( G->N(i) ) )
            ++pairs_count;
    }
    EXPECT_EQ( pairs_count, 31 );

    return;
}
