/*
 *  heuristic_run.h - runs a heuristic experiment
 */

#ifndef INCLUDE_HEURISTIC_RUNS_H_
#define INCLUDE_HEURISTIC_RUNS_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

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

void SetupAndRunHeuristic(std::string filename,
                          std::vector< chordalg::EliminationCriterion > criterion,
                          std::vector< chordalg::EliminationMode > modes,
                          bool atoms=false,
                          size_t runs=1,
                          float deficiency_wt=0,
                          float separation_wt=0);

class HeuristicRun {
 public:
    HeuristicRun() = delete;
    HeuristicRun(const HeuristicRun&) = delete;
    void operator=(const HeuristicRun&) = delete;

    HeuristicRun(const chordalg::Graph*, 
                 const std::vector< chordalg::EliminationParameters* >*,
                 bool,
                 size_t);
    virtual ~HeuristicRun() {}

    const std::vector< chordalg::VertexPair >& fill_edges() const;

    std::string Run();

 protected:
    const chordalg::Graph* const G_;
    const std::vector< chordalg::EliminationParameters* >* elimination_parameters_;
    bool atoms_;
    size_t runs_;

    std::vector< chordalg::VertexPair > fill_edges_;
};  // class HeuristicRun

#endif  // INCLUDE_HEURISTIC_RUNS_H_
