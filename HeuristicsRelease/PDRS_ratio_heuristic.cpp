#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

void PDRS_ratio_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename> [runs]"                                             << std::endl
                << "the optional ``runs'' indicates # of times the heuristic is run on each atom subgraph"  << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc < 2 ||  argc > 3 )
        PDRS_ratio_heuristic_usage( argv[0] );
//        std::cout << "usage: " << argv[0] << " <filename> [runs]" << std::endl;
    else
    {
        int runs = 0;
        if( argc == 3)
            runs = std::strtod( argv[2], NULL );

        RunAtomHeuristic<   ColoredIntersectionGraph,
                            MatrixCellIntGraphFR,
                            LBElimination,
                            RatioCriterion >
                            ( argv[ 1 ], new RatioCriterion(), runs );
    }

    return 1;
}
