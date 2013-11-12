#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

#include <cstdlib>
#include <cstring>
#include <string>

using namespace chordalg;

void PDRS_ratio_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename> <sep_weight> [runs]"                                << std::endl
                << "where ``sep_weight'' is a positive number or ``inf'' that weights"                      << std::endl
                << "the optional ``runs'' indicates # of times the heuristic is run on each atom subgraph"  << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc < 3 || argc > 4 )
        PDRS_ratio_heuristic_usage( argv[0] );
    else
    {
        Weight d, s;
        if( strncmp( argv[2], "inf", 3 ) == 0 )
        {
            d = 0;
            s = 1;
        }
        else
        {
            d = 1;
            s = std::strtod( argv[2], NULL );
        }

        int runs = 0;
        if( argc == 4 )
            runs = std::strtod( argv[3], NULL );

        std::cout << d << "*Deficiency(v) + " << s << "*Separation(v)" << std::endl;
        RunAtomHeuristic<   ColoredIntersectionGraph,
                            MatrixCellIntGraphFR,
                            LBElimination,
                            WSumCriterion >
                            ( argv[ 1 ], new WSumCriterion( d, s ), runs );
    }

    return 1;
}
