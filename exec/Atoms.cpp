#include "atom_subgraphs.hpp"
#include "file_reader.h"
#include "intersection_graph.h"

#include <cstdlib>
#include <string>
#include <sstream>

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        std::cout   << "usage: " << argv[0] << " <filename>"   << std::endl;
    }
    else
    {
        chordalg::MatrixCellIntGraphFR* graph_reader =
            chordalg::NewFileReader < chordalg::MatrixCellIntGraphFR >  ( std::string(argv[1]) );

        chordalg::ColoredIntersectionGraph G( graph_reader );
        chordalg::Atoms< chordalg::ColoredIntersectionGraph > A( G );
        int i = 0;
        for( chordalg::ColoredIntersectionGraph* a : A )
        {
            std::stringstream ss;
            ss << i;
            std::string suffix;
            if( a->IsClique() )
                suffix = std::string("_cliqueatom");
            else
                suffix = std::string("_atom");
            std::string atom_filename = std::string(argv[1]) + suffix + ss.str();
            a->Print(atom_filename);
            ++i;
        }
    }

    return 0;
}
