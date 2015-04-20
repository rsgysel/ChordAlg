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

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/vertices.h"
#include "heuristic_options.h"
#include "ChordAlgSrc/heuristic_run.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + atoms_opt + runs_opt;
    std::string filename;
    bool atoms = false;
    size_t runs = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs, &atoms);
    SetupAndRunHeuristic(
        filename,
        {EliminationCriterion::DEFICIENCY},
        {EliminationMode::CLASSIC},
        atoms,
        runs);
    return EXIT_SUCCESS;
}
