/*
 *  Mixed_heuristic - main program that implements a mixed
 *  elimination heuristic for maximum character compatibility.
 *  Scoring is done classically, and then lb-elimination is used.
 */

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "heuristic_options.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + atoms_opt + runs_opt; 
    std::string filename;
    bool atoms = false;
    size_t runs = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs, &atoms);
    chordalg::SetupAndRunHeuristic(
        filename,
        {EliminationCriterion::DEFICIENCY},
        {EliminationMode::MIXED},
        atoms,
        runs);
    return EXIT_SUCCESS;
}
