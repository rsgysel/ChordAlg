/*
 *  PDRS_wsum_heuristic - main program that implements a
 *  lb-elimination heuristic for maximum character compatibility.
 */

#include <cstdlib>
#include <cstring>

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "heuristic_options.h"
#include "ChordAlgSrc/heuristic_run.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    std::string usage = std::string(argv[0]) + preamble + file_opt + runs_opt + sep_opt;
    std::string filename;
    const bool atoms = true;
    size_t runs = 1;
    float def = 1, sep = 1;
    HeuristicOptions(argc, argv, usage, &filename, &runs, nullptr, &def, &sep);
    SetupAndRunHeuristic(
        filename,
        {EliminationCriterion::WSUM},
        {EliminationMode::LBELIMINATION},
        atoms,
        runs,
        def, 
        sep);
    return EXIT_SUCCESS;
}
