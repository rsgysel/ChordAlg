/*
 *  Mixed_heuristic - main program that implements a mixed
 *  elimination heuristic for maximum character compatibility.
 *  Scoring is done classically, and then lb-elimination is used.
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

#include <cstring>

#include <string>

#include "atom_subgraphs.hpp"
#include "mixed_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

void Mixed_heuristic_usage(std::string program) {
    std::cout << "usage: " << program << " <filename> (--atoms)" << std::endl;
    std::cout << "use --atoms to run the heuristic on atom subgraphs";
    std::cout << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        Mixed_heuristic_usage(argv[0]);
    } else {
        if (argc == 3 && strncmp(argv[2], "--atoms", 7) == 0) {
            RunAtomHeuristic<ColoredIntersectionGraph,
                             MatrixCellIntGraphFR,
                             MixedElimination,
                             RatioCriterion >(argv[1]);
        } else {
            RunHeuristic<ColoredIntersectionGraph,
                         MatrixCellIntGraphFR,
                         MixedElimination,
                         RatioCriterion >(argv[1]);
        }
    }
    return 0;
}
