/*
 *  MMC_MaxCompatSupertree - main program that constructs a supertree
 *  from an MRP matrix representing a collection of partially labeled
 *  trees. Utilizes MMC heuristic to estimate maximum compatibility.
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. ÊSee the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. ÊIf not, see <http://www.gnu.org/licenses/>.
 */

#include "atom_subgraphs.hpp"
#include "classic_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "mcs.h"
#include "tree_representation.h"

#include <cstring>

using namespace chordalg;


void MMC_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename>"                    << std::endl
                << "where filename is a MRP matrix file in NEXUS format"    << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc != 2 )
        MMC_heuristic_usage( argv[0] );
    else
    {
        NexusMRPFR* graph_reader    = NewFileReader < NexusMRPFR >  ( argv[1] );
        ColoredIntersectionGraph G( graph_reader );
        ClassicElimination eo( G, new DeficiencyCriterion() );
        chordalg::Supergraph triangulation(G,eo.TriangNbhds());
        chordalg::CliqueTree* ct = chordalg::MCSCliqueTree(triangulation);
        ct->PhyloNewickPrint(G,true);

        std::cout << std::endl;
        eo.PrettyPrint();
        std::cout << "fill weight: "    << eo.fill_cost()   << std::endl;
        std::cout << "fill count: "     << eo.fill_count()  << std::endl;
        std::cout << "vertices: "       << G.order()        << std::endl;
        std::cout << "edges: "          << G.size()         << std::endl;
    }

    return EXIT_SUCCESS;
}

