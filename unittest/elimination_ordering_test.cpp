#include "atom_subgraphs.hpp"
#include "elimination_order.h"
#include "file_reader.h"
#include "graph_test.h"
#include "utilities.hpp"

#include "classic_heuristics.h"
#include "separation_heuristics.h"

#include "lb_triang_heuristic.h"
#include "elimination_heuristics.h"
#include "elimination_ordering.h"

TEST_F( MatrixCellIntGraphTest, Debug )
{
    using namespace chordalg;
    MatrixCellIntGraphFR* graph_reader =
 //       NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "BadExample_2state20.20-0.2.dat" ) );
        NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "18232state50.50-0.1.npp" ) );
    ColoredIntersectionGraph G( graph_reader );
    double old_classic = 0, new_classic = 0, old_lb = 0, new_lb = 0;

    Atoms< ColoredIntersectionGraph > A( G );

int i = 0;
    for( ColoredIntersectionGraph* a : A )
    {

//        MonochromaticFillPairHeuristic eo( *a );
//        old_classic += eo.fill_weight();
//
//        MonochromaticDeficiencyHeuristic new_eo( *a );
//        new_classic += new_eo.fill_cost();

//std::cout << std::endl << "Old: " << std::endl;
//        LBTriangHeuristic lbeo( *a );
//        old_lb += lbeo.fill_weight();
//std::cout << std::endl << "New: " << std::endl;
//        MonochromaticPDRS new_lbeo( *a );
//        new_lb += new_lbeo.fill_cost();

    if( i == 5 )
    {

std::cout << std::endl << "Old: " << std::endl;
        LBTriangHeuristic lbeo( *a );
        old_lb += lbeo.fill_weight();
std::cout << std::endl << "New: " << std::endl;
        MonochromaticPDRS new_lbeo( *a );
        new_lb += new_lbeo.fill_cost();
            a->PrettyPrint();
            for( Vertex v : *a )
            {
                std::cout << "there are " << a->vertex_color(v).size() << " colors of " << a->name(v) << " " << std::endl;
                std::copy( a->vertex_color(v).begin(), a->vertex_color(v).end(), std::ostream_iterator<Color>( std::cout, " " )  );
                std::cout << std::endl;
            }
    }

//        if(i == 3)
//            eo.PrettyPrint();

        ++i;
    }

    std::cout << "classic fill weights: " << old_classic << " and " << new_classic << std::endl;
    std::cout << "lb fill weights: " << old_lb << " and " << new_lb << std::endl;
    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillMaxSeparateTest )
{
    Read( graph_dir() + std::string( "minfill_test.m" ) );

    chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( *H );
    for( chordalg::ColoredIntersectionGraph* a : A )
    {
        chordalg::LBTriangHeuristic eo( *a );
    }

    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillFile )
{
    chordalg::AnalyzeFile<  chordalg::ColoredIntersectionGraph,
                        chordalg::MatrixCellIntGraphFR,
                        chordalg::MonochromaticFillPairHeuristic >
                        ( "graphfiles/minfill_test.m" );
    return;
}

TEST_F( SortedAdjacencyListTest, EliminationOrderStar )
{
    Read( graph_dir() + std::string( "star.sal" ) );
    chordalg::EliminationOrdering pi( *G );
    EXPECT_EQ( pi.fill_size(), 15 );
    pi.Swap( 0 ,4 );       EXPECT_EQ( pi.fill_size(), 1 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 4, 0 );       EXPECT_EQ( pi.fill_size(), 15 );   EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 0, 6 );       EXPECT_EQ( pi.fill_size(), 0 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 6, 0 );       EXPECT_EQ( pi.fill_size(), 15 );   EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 0, 3 );       EXPECT_EQ( pi.fill_size(), 3 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 3, 4 );       EXPECT_EQ( pi.fill_size(), 1 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 4, 5 );       EXPECT_EQ( pi.fill_size(), 0 );    EXPECT_EQ( pi.IsOrderConsistent(), true );

    pi.MoveAndPush( 2, 3 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 2, 5 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 4, 6 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 1, 3 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 0, 2 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 1, 5 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.MoveAndPush( 4, 5 );    EXPECT_EQ( pi.IsOrderConsistent(), true );

    return;
}

TEST_F( SortedAdjacencyListTest, EliminationOrderTree )
{
    Read( graph_dir() + std::string( "tree.sal" ) );
    chordalg::EliminationOrdering pi(*G);
    EXPECT_EQ( pi.fill_size(), 0 );    EXPECT_EQ( pi.IsOrderConsistent(), true );
    pi.Swap( 0, 13 );
    EXPECT_EQ( pi.fill_size(), 4 );    EXPECT_EQ( pi.IsOrderConsistent(), true );

    return;
}

TEST_F( MatrixCellIntGraphTest, MinfillChordalTest )
{

    Read( graph_dir() + std::string( "cig_test.m" ) );
    chordalg::MonochromaticFillPairHeuristic eo( *H );

    return;
}

TEST_F( MatrixCellIntGraphTest, MinfillUnchordalTest )
{

    Read( graph_dir() + std::string( "minfill_test.m" ) );
    chordalg::MonochromaticFillPairHeuristic eo( *H );

//// need to turn off tie breaking for this
//    chordalg::VertexContainer pi = { 0, 4, 1, 5, 6, 7, 2, 3, 8 };
//    EXPECT_EQ( eo.Ordering() == pi, true );

    return;
}

