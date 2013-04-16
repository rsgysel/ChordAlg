#include "chordalg_types.h"
#include "graph_test.h"

#include "atom_subgraphs.hpp"

TEST_F( MatrixCellIntGraphTest, FileReaderTest )
{
    Read( graph_dir() + std::string( "cig_test.m" ) );
    chordalg::Graph H( chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >( graph_dir() + std::string( "cig_test.sal" ) ) );
    EXPECT_EQ( G->IsIsomorphic( H ), true );
    return;
}

TEST_F( MatrixCellIntGraphTest, AtomsOfCig )
{
    Read( graph_dir() + std::string( "cig_test.m" ) );

    chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( *H );
    for( chordalg::ColoredIntersectionGraph* a : A )
    {
        for( chordalg::Vertex v : *H )
        {
            for( chordalg::Vertex u : *a )
            {
                if( H->name( v ) == a->name( u ) )
                {
                    const chordalg::Subset& subset = a->subset( u );
                    bool H_has_subset = H->subset_family()->Contains< chordalg::Subset >( subset );
                    EXPECT_EQ( H_has_subset , true );
                }
            }
        }
    }

    return;
}
