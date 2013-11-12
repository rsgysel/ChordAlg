#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

int main( int argc, char* argv[] )
{
    if( argc != 2 && argc != 3 )
    {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    }
    else
    {
        MatrixCellIntGraphFR* graph_reader    = NewFileReader < MatrixCellIntGraphFR >  ( argv[1] );
        ColoredIntersectionGraph G( graph_reader );
        std::cout << "vertices: "       << G.order()    << std::endl;
        std::cout << "edges: "          << G.size()     << std::endl;
    }
}
