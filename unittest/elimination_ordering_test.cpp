#include "atom_subgraphs.hpp"
#include "classic_heuristics.h"
#include "elimination_order.h"
#include "file_reader.h"
#include "graph_test.h"
#include "separation_heuristics.h"
#include "utilities.hpp"

TEST_F( MatrixCellIntGraphTest, Debug )
{
    using namespace chordalg;
    MatrixCellIntGraphFR* graph_reader =
 //       NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "BadExample_2state20.20-0.2.dat" ) );
        NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "18232state50.50-0.1.npp" ) );
    ColoredIntersectionGraph G( graph_reader );

    Atoms< ColoredIntersectionGraph > A( G );

//    Cost classic = 0, lb = 0;

    int i = 0;
    for( ColoredIntersectionGraph* a : A )
    {
        if( i < 5)
            EXPECT_EQ( a->IsClique(), true );
        else
            EXPECT_EQ( a->IsClique(), false );
//            MonochromaticDeficiencyHeuristic new_eo( *a );
//            classic += new_eo.fill_cost();
//
//            MonochromaticPDRS new_lbeo( *a );
//            lb += new_lbeo.fill_cost();
        ++i;
    }

    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillMaxSeparateTest )
{
    Read( graph_dir() + std::string( "minfill_test.m" ) );

    chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( *H );
    for( chordalg::ColoredIntersectionGraph* a : A )
    {
        chordalg::MonochromaticPDRS eo( *a );
    }

    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillFile )
{
    chordalg::AnalyzeFile<  chordalg::ColoredIntersectionGraph,
                        chordalg::MatrixCellIntGraphFR,
                        chordalg::MonochromaticDeficiencyHeuristic >
                        ( "graphfiles/minfill_test.m" );
    return;
}

TEST_F( MatrixCellIntGraphTest, MinfillChordalTest )
{

    Read( graph_dir() + std::string( "cig_test.m" ) );
    chordalg::MonochromaticDeficiencyHeuristic eo( *H );

    return;
}

TEST_F( MatrixCellIntGraphTest, MinfillUnchordalTest )
{

    Read( graph_dir() + std::string( "minfill_test.m" ) );
    chordalg::MonochromaticDeficiencyHeuristic eo( *H );

//// need to turn off tie breaking for this
//    chordalg::VertexVector pi = { 0, 4, 1, 5, 6, 7, 2, 3, 8 };
//    EXPECT_EQ( eo.Ordering() == pi, true );

    return;
}

