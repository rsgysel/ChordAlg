/*
 *  MMC_heuristic - main program that implements a classic
 *  elimination heuristic for maximum character compatibility.
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
    std::string columns = chordalg::SetupAndRunHeuristic(
        filename,
        {EliminationCriterion::DEFICIENCY},
        {EliminationMode::CLASSIC},
        atoms,
        runs);
    std::cout << "Columns removed:\n" << columns << std::endl;
    return EXIT_SUCCESS;
}
