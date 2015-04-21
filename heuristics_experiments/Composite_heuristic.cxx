/*
 *  Composite_herusitic.cpp - main program that implements classic and
 *  lb-elimination heuristics for maximum character compatibility
 */

#include <cstring>

#include <algorithm>
#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "heuristic_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + runs_opt + sep_opt;
    std::string filename;
    const bool atoms = true;
    size_t runs = 1;
    float def = 1, sep = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs, nullptr, &def, &sep);
    chordalg::SetupAndRunHeuristic(
        filename,
        {   EliminationCriterion::DEFICIENCY, 
            EliminationCriterion::RATIO,
            EliminationCriterion::WSUM},
        {   EliminationMode::CLASSIC,
            EliminationMode::LBELIMINATION,
            EliminationMode::MIXED},
        atoms,
        runs,
        def,
        sep);
    return EXIT_SUCCESS;
}
