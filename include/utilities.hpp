#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include "file_reader.h"
#include "elimination_heuristics.h"
#include "atom_subgraphs.hpp"
#include "intersection_graph.h"

namespace chordalg {

template< class GraphType, class FileReaderType, class HeuristicType >
void AnalyzeFile( std::string filename )
{
    FileReaderType* graph_reader = NewFileReader< FileReaderType >( filename );

    GraphType G( graph_reader );
    Weight total_weight = 0;

    Atoms< GraphType > A( G );

    for( GraphType* a : A )
    {
        HeuristicType eo( *a );
        total_weight += eo.fill_weight();
    }

    std::cout << "fill weight: " << total_weight << std::endl;
    return;
}

} // namespace chordalg

#endif // UTILITIES_HPP_INCLUDED
