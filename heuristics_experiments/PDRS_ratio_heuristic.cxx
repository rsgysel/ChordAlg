/*
 *  PDRS_ratio_heuristic - main program that implements a
 *  lb-elimination heuristic for maximum character compatibility.
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "heuristic_options.h"
#include "ChordAlgSrc/heuristic_run.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + runs_opt;
    std::string filename;
    const bool atoms = true;
    size_t runs = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs);
    SetupAndRunHeuristic(
        filename,
        {EliminationCriterion::RATIO},
        {EliminationMode::LBELIMINATION},
        atoms,
        runs);
    return EXIT_SUCCESS;
}
