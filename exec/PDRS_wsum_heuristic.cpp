#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "utilities.hpp"

#include <cstdlib>
#include <string>

using namespace chordalg;

int main( int argc, char* argv[] )
{
    if( argc != 3 )
    {
        std::cout   << "usage: " << argv[0] << " <filename> <sep_weight>"   << std::endl
                    << "where ``sep_weight'' is a real number or ``inf''"   << std::endl;
    }
    else
    {
        Weight d, s;
        if( argv[2] == "inf" )
        {
            d = 0;
            s = 1;
        }
        else
        {
            d = 1;
            s = std::strtod( argv[2], NULL );
        }

        std::cout << d << "*Deficiency(v) + " << s << "*Separation(v)" << std::endl;
        RunHeuristic<   ColoredIntersectionGraph,
                        MatrixCellIntGraphFR,
                        LBElimination,
                        WSumCriterion >
                        ( argv[ 1 ], new WSumCriterion( d, s ) );
    }

    return 1;
}
