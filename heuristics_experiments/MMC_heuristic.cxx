/*
 *  MMC_heuristic - main program that implements a classic
 *  elimination heuristic for maximum character compatibility.
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/vertices.h"

using namespace chordalg;

void MMC_heuristic_usage(std::string program) {
    std::cerr << "usage: " << program << " <filename> [-a, runs]";
    std::cerr << std::endl;
    std::cerr << "the optional ``-a'' computes atom subgraphs before ";
    std::cerr << "running the heuristic" << std::endl;
    std::cerr << "the optional ``runs'' indicates # of the times heuristic ";
    std::cerr << "is run on each atom subgraph" << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        MMC_heuristic_usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        if (argc == 2) {
            RunHeuristic<ColoredIntersectionGraph,
                         MatrixCellIntGraphFR,
                         ClassicElimination,
                         DeficiencyCriterion >(argv[1]);
        } else if (std::string("-a") == std::string(argv[2])) {
            int runs = 0;
            if (argc == 4) {
                runs = std::strtod(argv[3], NULL);
            }
            RunAtomHeuristic<ColoredIntersectionGraph,
                             MatrixCellIntGraphFR,
                             ClassicElimination,
                             DeficiencyCriterion >
                             (argv[1], new DeficiencyCriterion(), runs);
        } else {
            MMC_heuristic_usage(argv[0]);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}
