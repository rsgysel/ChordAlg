/*
 *  Composite_herusitic.cpp - main program that implements classic and
 *  lb-elimination heuristics for maximum character compatibility
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

#include <algorithm>
#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "heuristic_options.h"
#include "heuristic_runs.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + run_opt + sep_opt;
    std::string filename;
    const bool atoms = true;
    size_t runs = 1;
    float def = 1, sep = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs, nullptr, &def, &sep);
    SetupAndRunHeuristic(
        filename,
        {"ClassicElimination", "LBElimination", "MixedElimination"},
        EliminationCriterion::DEFICIENCY, // need to do all criteria...
        atoms,
        runs,
        def,
        sep);
    return EXIT_SUCCESS;
}
