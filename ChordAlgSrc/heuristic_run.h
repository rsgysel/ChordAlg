/*
 *  heuristic_run.h - runs a heuristic experiment
 */

#ifndef INCLUDE_HEURISTIC_RUNS_H_
#define INCLUDE_HEURISTIC_RUNS_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <set>
#include <string>
#include <vector>
#include <utility>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

std::string SetupAndRunHeuristic(
    std::string filename,
    std::vector< EliminationCriterion > criterion,
    std::vector< EliminationMode > modes,
    bool atoms=false,
    size_t runs=1,
    float deficiency_wt=0,
    float separation_wt=0);

class HeuristicRun {
 public:
    HeuristicRun() = delete;
    HeuristicRun(const HeuristicRun&) = delete;
    void operator=(const HeuristicRun&) = delete;

    HeuristicRun(const Graph*, 
                 const std::vector< EliminationParameters* >*,
                 bool,
                 size_t);
    virtual ~HeuristicRun() {}

    std::vector< VertexPair > fill_edges() const;

    std::string Run();
    AdjacencyLists* TriangNbhds() const;
    Weight fill_weight() const {
        return fill_weight_;
    }
 protected:
    const Graph* const G_;
    const std::vector< EliminationParameters* >* elimination_parameters_;
    bool atoms_;
    size_t runs_;

    std::vector< VertexPair > fill_edges_;
    Weight fill_weight_;
};  // HeuristicRun

}  // namespace chordalg

#endif  // INCLUDE_HEURISTIC_RUNS_H_
