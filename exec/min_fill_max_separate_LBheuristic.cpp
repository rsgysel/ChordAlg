#include "atom_subgraphs.hpp"
#include "lb_triang_heuristic.h"
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
        AnalyzeFile<    ColoredIntersectionGraph,
                        MatrixCellIntGraphFR,
                        LBTriangHeuristic >
                        ( argv[ 1 ] );
    }

    return 1;
}
