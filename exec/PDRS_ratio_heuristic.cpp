#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "utilities.hpp"

using namespace chordalg;

int main( int argc, char* argv[] )
{
    if( argc != 2 )
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    else
    {
        RunHeuristic<   ColoredIntersectionGraph,
                        MatrixCellIntGraphFR,
                        LBElimination,
                        RatioCriterion >
                        ( argv[ 1 ] );
    }

    return 1;
}
