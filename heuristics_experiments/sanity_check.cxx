/*
 *  Composite_herusitic.cpp - main program that implements classic and
 *  lb-elimination heuristics for maximum character compatibility
 */

#include <cassert>
#include <cstring>

#include <algorithm>
#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/triangulation.h"
#include "heuristic_options.h"

using namespace chordalg;
void CheckHeuristic(std::string, EliminationCriterion, EliminationMode);

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + preamble + file_opt;
    std::string filename;
    HeuristicOptions(argc, argv, usage, &filename, 
                     nullptr, nullptr, nullptr, nullptr);
    std::cout << "Checking MMC...";
    CheckHeuristic(filename,
                   EliminationCriterion::DEFICIENCY,
                   EliminationMode::CLASSIC);
    std::cout << "Done.\nChecking LBRatio...";
    CheckHeuristic(filename,
                   EliminationCriterion::RATIO,
                   EliminationMode::LBELIMINATION);
    std::cout << "Done.\nChecking LBWsum...";
    CheckHeuristic(filename,
                   EliminationCriterion::WSUM,
                   EliminationMode::LBELIMINATION);
    std::cout << "Done.\nChecking Mixed...";
    CheckHeuristic(filename,
                   EliminationCriterion::DEFICIENCY,
                   EliminationMode::MIXED);
    std::cout << "Done.\nAll checks complete.\n";
    return EXIT_SUCCESS;
}

void CheckHeuristic(std::string filename,
                    EliminationCriterion criterion,
                    EliminationMode mode) {
    const bool atoms = true;
    const size_t runs = 1;
    const float def = 1, sep = 1;
    // Run Hueristic
    CellIntersectionGraph* G = CellIntersectionGraph::New(filename);
    std::vector< chordalg::EliminationParameters* > elimination_parameters;
    elimination_parameters.push_back(
        new chordalg::EliminationParameters(criterion, mode, def, sep));
    HeuristicRun R(G, &elimination_parameters, atoms, runs);
    R.Run();
    // Check Triangulation
    Triangulation H(G, R.fill_edges());
    assert(H.IsChordal());
    // Check Columns to remove
    size_t columns = 0;
    for (Vertex v : *G) {
        for (Vertex u : (*R.fill_edges())[v]) {
            if (u < v) {
                columns += G->CommonColorCount(u, v);
            }
        }
    }
    assert(R.fill_edges()->fill_weight() == columns);
    delete G;
    delete elimination_parameters[0];
}
