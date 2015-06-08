/*
 *  heuristic_run.h - runs a heuristic experiment / computation
 */

#ifndef CHORDALGSRC_HEURISTIC_RUN_H_
#define CHORDALGSRC_HEURISTIC_RUN_H_

#include <initializer_list>
#include <string>
#include <vector>

#include "ChordAlgSrc/elimination_heuristic.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

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

    const FillEdges& fill_edges() const;
    const std::string& fill_summary() const;
    const std::string& run_summary() const;
    static HeuristicRun* New(
        const CellIntersectionGraph*,
        std::initializer_list< EliminationCriterion >,
        std::initializer_list< EliminationMode >,
        bool atoms = false,
        size_t runs = 1,
        float deficiency_wt = 0,
        float separation_wt = 0);
    void Run();

 protected:
    static bool Compatible(EliminationMode, EliminationCriterion);

    const Graph* const G_;
    const std::vector< EliminationParameters* >* elimination_parameters_;
    bool atoms_;
    size_t runs_;
    FillEdges* fill_edges_;
    std::string fill_summary_;
    std::string run_summary_;
};  // HeuristicRun

}  // namespace chordalg

#endif  // CHORDALGSRC_HEURISTIC_RUN_H_
