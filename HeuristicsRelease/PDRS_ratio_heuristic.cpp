/*
 *  PDRS_ratio_heuristic - main program that implements a
 *  lb-elimination heuristic for maximum character compatibility.
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

#include <cstring>

using namespace chordalg;

void PDRS_ratio_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename> [runs]"                                             << std::endl
                << "the optional ``runs'' indicates # of times the heuristic is run on each atom subgraph"  << std::endl;
    return;
}

int main( int argc, char* argv[] )
{
    if( argc < 2 ||  argc > 3 )
        PDRS_ratio_heuristic_usage( argv[0] );
//        std::cout << "usage: " << argv[0] << " <filename> [runs]" << std::endl;
    else
    {
        int runs = 0;
        if( argc == 3)
            runs = std::strtod( argv[2], NULL );

        RunAtomHeuristic<   ColoredIntersectionGraph,
                            MatrixCellIntGraphFR,
                            LBElimination,
                            RatioCriterion >
                            ( argv[ 1 ], new RatioCriterion(), runs );
    }

    return 1;
}