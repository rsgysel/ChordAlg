#include "atom_subgraphs.hpp"
#include "mixed_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

#include <cstring>

using namespace chordalg;

void Mixed_heuristic_usage( std::string program )
{
    std::cout << "usage: " << program << " <filename> (--atoms)" << std::endl;
    std::cout << "use --atoms to run the heuristic on atom subgraphs" << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc != 2 && argc != 3 )
        Mixed_heuristic_usage( argv[0] );
    else
    {
        if( argc == 3 && strncmp( argv[2], "--atoms", 7 ) == 0 )
        {
            RunAtomHeuristic<   ColoredIntersectionGraph,
                                    MatrixCellIntGraphFR,
                                    MixedElimination,
                                    RatioCriterion >
                                    ( argv[ 1 ] );
        }
        else
        {
            RunHeuristic<   ColoredIntersectionGraph,
                            MatrixCellIntGraphFR,
                            MixedElimination,
                            RatioCriterion >
                            ( argv[ 1 ] );

        }
    }

    return 0;
}
