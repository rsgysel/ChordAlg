#include "atom_subgraphs.hpp"
#include "classic_elimination.h"
#include "elimination_order.h"
#include "file_reader.h"
#include "graph_test.h"
#include "lb_elimination.h"
#include "mixed_elimination.h"

TEST_F( MatrixCellIntGraphTest, Debug )
{
    using namespace chordalg;
    MatrixCellIntGraphFR* graph_reader =
 //       NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "BadExample_2state20.20-0.2.dat" ) );
        NewFileReader< MatrixCellIntGraphFR >( graph_dir() + std::string( "18232state50.50-0.1.npp" ) );
    ColoredIntersectionGraph G( graph_reader );

    Atoms< ColoredIntersectionGraph > A( G );

    Cost classic = 0, lb = 0;

    int i = 0;
    for( ColoredIntersectionGraph* a : A )
    {
        if( i < 5)
            EXPECT_EQ( a->IsClique(), true );
        else
        {
            EXPECT_EQ( a->IsClique(), false );

            ClassicElimination eo( *a, new DeficiencyCriterion() );
            classic += eo.fill_cost();
            LBElimination lb_eo( *a, new RatioCriterion() );
            lb += lb_eo.fill_cost();
        }
        ++i;
    }
    std::cout << "Classic: " << classic << " lb: " << lb << std::endl;
    EXPECT_EQ( classic, 19 );
    EXPECT_EQ( lb, 19 );

    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillMaxSeparateTest )
{
    Read( graph_dir() + std::string( "minfill_test.m" ) );

    chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( *H );
    for( chordalg::ColoredIntersectionGraph* a : A )
    {
        chordalg::LBElimination eo( *a, new chordalg::RatioCriterion() );
    }

    return;
}

TEST_F( MatrixCellIntGraphTest, MinFillFile )
{
    using namespace chordalg;
    RunHeuristic<   ColoredIntersectionGraph,
                    MatrixCellIntGraphFR,
                    ClassicElimination,
                    DeficiencyCriterion >
                    ( graph_dir() + "minfill_test.m" );

    return;
}


TEST_F( MatrixCellIntGraphTest, MixedElimFillFile )
{
    using namespace chordalg;
    RunHeuristic<   ColoredIntersectionGraph,
                    MatrixCellIntGraphFR,
                    MixedElimination,
                    RatioCriterion >
                    ( graph_dir() + "minfill_test.m" );

    return;
}


TEST_F( MatrixCellIntGraphTest, MinfillChordalTest )
{

    Read( graph_dir() + std::string( "cig_test.m" ) );
    chordalg::ClassicElimination eo( *H, new chordalg::DeficiencyCriterion() );

    return;
}

TEST_F( MatrixCellIntGraphTest, MixedElimCigTest )
{

    Read( graph_dir() + std::string( "cig_test.m" ) );
    chordalg::MixedElimination eo( *H, new chordalg::RatioCriterion() );

    return;
}

TEST_F( MatrixCellIntGraphTest, MinfillUnchordalTest )
{

    Read( graph_dir() + std::string( "minfill_test.m" ) );
    chordalg::ClassicElimination eo( *H, new chordalg::DeficiencyCriterion() );

//// need to turn off tie breaking for this
//    chordalg::VertexVector pi = { 0, 4, 1, 5, 6, 7, 2, 3, 8 };
//    EXPECT_EQ( eo.Ordering() == pi, true );

    return;
}

