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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>

#include <string>

#include "classic_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

void MMC_heuristic_usage(std::string program) {
    std::cout << "usage: " << program << " <filename> [-a, runs]";
    std::cout << std::endl;
    std::cout << "the optional ``-a'' computes atom subgraphs before ";
    std::cout << "running the heuristic" << std::endl;
    std::cout << "the optional ``runs'' indicates # of the times heuristic ";
    std::cout << "is run on each atom subgraph" << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        MMC_heuristic_usage(argv[0]);
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
        }
    }
    return 1;
}
