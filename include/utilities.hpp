#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include "file_reader.h"
#include "atom_subgraphs.hpp"
#include "intersection_graph.h"

#include <algorithm>
#include <iostream>

namespace chordalg {

template< class GraphType, class FileReaderType, class HeuristicType, class CriterionType >
void RunHeuristic( std::string filename, CriterionType* criterion = new CriterionType() )
{
    FileReaderType* graph_reader    = NewFileReader < FileReaderType >  ( filename );

    GraphType G( graph_reader );
    Weight total_weight = 0;

    Atoms< GraphType > A( G );
    int clique_atoms = 0, atom_id = 0, total_count = 0;
    for( GraphType* a : A )
    {
        ++atom_id;
        if( !a->IsClique() )
        {
            HeuristicType eo( *a, criterion );
            total_weight    +=  eo.fill_cost();
            total_count     +=  eo.fill_count();
            std::cout << "atom " << atom_id << std::endl;
            eo.PrettyPrint();
        }
        else
            ++clique_atoms;
    }

    std::cout << "fill weight: "    << total_weight << std::endl;
    std::cout << "fill count: "     << total_count  << std::endl;
    std::cout << "vertices: "       << G.order()    << std::endl;
    std::cout << "edges: "          << G.size()     << std::endl;
    std::cout << "atoms: "          << A.size()     << std::endl;
    std::cout << "clique atoms: "   << clique_atoms << std::endl;

    return;
}

} // namespace chordalg

#endif // UTILITIES_HPP_INCLUDED
