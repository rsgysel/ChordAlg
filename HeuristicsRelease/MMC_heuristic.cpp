#include "atom_subgraphs.hpp"
#include "classic_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

void MMC_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename> [-a, runs]"                                         << std::endl
                << "the optional ``-a'' computes atom subgraphs before running the heuristic"               << std::endl
                << "the optional ``runs'' indicates # of the times heuristic is run on each atom subgraph"  << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc < 2 || argc > 4 )
        MMC_heuristic_usage( argv[0] );
    else
    {
        if( argc == 2 )
        {
            RunHeuristic<   ColoredIntersectionGraph,
                            MatrixCellIntGraphFR,
                            ClassicElimination,
                            DeficiencyCriterion >
                            ( argv[ 1 ] );

        }
        else if( std::string("-a") == std::string(argv[2]) )
        {
            int runs = 0;
            if( argc == 4)
                runs = std::strtod( argv[3], NULL );

            RunAtomHeuristic<   ColoredIntersectionGraph,
                                MatrixCellIntGraphFR,
                                ClassicElimination,
                                DeficiencyCriterion >
                                ( argv[ 1 ], new DeficiencyCriterion(), runs );

        }
        else
            MMC_heuristic_usage( argv[0] );
    }

    return 1;
}
