/*
 *  heuristic_run.h - runs a heuristic experiment
 */

#ifndef CHORDALGSRC_HEURISTIC_RUN_H_
#define CHORDALGSRC_HEURISTIC_RUN_H_

#include <string>
#include <vector>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

std::string SetupAndRunHeuristic(
    std::string filename,
    std::vector< EliminationCriterion > criterion,
    std::vector< EliminationMode > modes,
    bool atoms = false,
    size_t runs = 1,
    float deficiency_wt = 0,
    float separation_wt = 0);

class HeuristicRun {
 public:
    HeuristicRun() = delete;
    HeuristicRun(const HeuristicRun&) = delete;
    void operator=(const HeuristicRun&) = delete;

    HeuristicRun(
        const Graph*,
        const std::vector< EliminationParameters* >*,
        bool,
        size_t);
    ~HeuristicRun();

    const FillEdges* fill_edges() const;
    std::string Run();

 protected:
    const Graph* const G_;
    const std::vector< EliminationParameters* >* elimination_parameters_;
    bool atoms_;
    size_t runs_;

    FillEdges* fill_edges_;
};  // HeuristicRun

}  // namespace chordalg

#endif  // CHORDALGSRC_HEURISTIC_RUN_H_
