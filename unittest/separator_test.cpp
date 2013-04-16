#include "chordalg_types.h"
#include "graph_test.h"
#include "lex_trie.h"
#include "separator.h"

// Tests connected component calculation
TEST_F( SortedAdjacencyListTest, SeparatorComponents )
{
    Read( graph_dir() + std::string( "cc_test.sal" ) );
    EXPECT_EQ( G->order(), 15 );
    EXPECT_EQ( G->size(), 23 );

    chordalg::SeparatorComponents S( *G );
    chordalg::VertexContainer X = { 0, 1, 2, 3 };
    S.Separate( X );
    EXPECT_EQ( S.size(), 4 );

    // now check connected component consistency. subroutine below is independent of cc_test.sal
    for( chordalg::Vertex v : *G )
    {
        EXPECT_NE( S.ComponentId( v ), S.kUnsearched() );
        if( !S.IsInSeparator( v ) )
        {
            for( chordalg::Vertex u : G->N( v ) )
            {
                if( !S.IsInSeparator( u ) )
                    EXPECT_EQ( S.ComponentId( v ), S.ComponentId( u ) );
            } // neighbors not in S are in same connected component
        } // for vertices not in S
    }

    chordalg::SeparatorBlocks Sblocks( *G );
    Sblocks.Separate( X );
    int i = 0, sizes[] = { 4, 1, 2, 4 };
    for( const chordalg::VertexContainer& NC : Sblocks )
        EXPECT_EQ( NC.size(), static_cast< unsigned int >( sizes[ i++ ] ) );

    Sblocks.Separate( X );
    i = 0;
    for( const chordalg::VertexContainer& NC : Sblocks )
        EXPECT_EQ( NC.size(), static_cast< unsigned int >( sizes[ i++ ] ) );

    return;
}

