/*
 *  elimination_heuristics
 *
 *  Checks that the elimination heuristics for character
 *  compatibility satisfy the following:
 *  1) A heuristic run results in a triangulation
 *  2) An LB-Elimination run results in a triangulation
 *  3) The weight of fill edges added = the number of removed characters
 *
 */

#include <cassert>
#include <string>

#include "ChordAlgSrc/elimination_heuristic.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;
void CheckHeuristic(std::string, EliminationCriterion, EliminationMode);

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + " <argv[1]>";
    if (argc != 2) {
        std::cerr << usage << '\n';
        exit(EXIT_FAILURE);
    }
    std::cout << "Checking MMC...";
    CheckHeuristic(argv[1],
                   EliminationCriterion::DEFICIENCY,
                   EliminationMode::CLASSIC);
    std::cout << "Done.\nChecking LBRatio...";
    CheckHeuristic(argv[1],
                   EliminationCriterion::RATIO,
                   EliminationMode::LBELIMINATION);
    std::cout << "Done.\nChecking LBWsum...";
    CheckHeuristic(argv[1],
                   EliminationCriterion::WSUM,
                   EliminationMode::LBELIMINATION);
    std::cout << "Done.\nChecking Mixed...";
    CheckHeuristic(argv[1],
                   EliminationCriterion::DEFICIENCY,
                   EliminationMode::MIXED);
    std::cout << "Done.\nAll checks successfully completed.\n";
    return EXIT_SUCCESS;
}

void CheckHeuristic(std::string filename,
                    EliminationCriterion criterion,
                    EliminationMode mode) {
    const bool atoms = true;
    const size_t runs = 1;
    const float def = 1, sep = 1;

    // Run Heuristic
    CellIntersectionGraph* G = CellIntersectionGraph::New(filename);
    std::vector< chordalg::EliminationParameters* > elimination_parameters;
    elimination_parameters.push_back(
        new chordalg::EliminationParameters(criterion, mode, def, sep));
    HeuristicRun R(G, &elimination_parameters, atoms, runs);
    R.Run();

    // Check Triangulation
    Triangulation* H = Triangulation::New(G, R.fill_edges());
    assert(H->IsTriangulated());

    // Check Minimality of Triangulation
    if (mode == EliminationMode::LBELIMINATION) {
        assert(H->IsMinimalTriangulation());
    }

    // Check Columns to remove
    size_t columns = 0;
    for (Vertex v : *G) {
        for (Vertex u : R.fill_edges()[v]) {
            if (u < v) {
                columns += G->CommonColorCount(u, v);
            }
        }
    }
    assert(R.fill_edges().fill_weight() == columns);
    delete H;
    delete G;
    delete elimination_parameters[0];
}
