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

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"

using namespace chordalg;

void Mixed_heuristic_usage(std::string program) {
    std::cerr << "usage: " << program << " <filename> [-a]" << std::endl;
    std::cerr << "use -a to run the heuristic on atom subgraphs";
    std::cerr << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        Mixed_heuristic_usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        if (argc == 3 && std::string("-a").compare(argv[2]) == 0) {
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
    return EXIT_SUCCESS;
}
