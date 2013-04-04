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
    H->PrettyPrint();
    H->PrettyPrintSubsets();

    chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( *H );
    for( chordalg::ColoredIntersectionGraph* a : A )
    {
        a->PrettyPrint();
        a->PrettyPrintSubsets();
    }

    return;
}
