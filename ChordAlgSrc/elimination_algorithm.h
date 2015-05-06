/*
 *  elimination_algorithm.h - base class for vertex elimination algorithms
 */

#ifndef CHORDALGSRC_ELIMINATION_ALGORITHM_H_
#define CHORDALGSRC_ELIMINATION_ALGORITHM_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

enum class EliminationCriterion {
    DEFICIENCY,         // #deficient pairs needed to saturate nbhd
    RATIO,              // #separated pairs / #deficient pairs
    WSUM                // #separated pairs - d #deficient pairs
};  // EliminationCriterion

enum class EliminationMode {
    CLASSIC,
    LBELIMINATION,
    MIXED
};  // EliminationMode

class EliminationParameters {
 public:
    EliminationParameters() = delete;
    EliminationParameters(const EliminationParameters&) = delete;
    void operator=(const EliminationParameters&) = delete;

    explicit EliminationParameters(
        EliminationCriterion,
        EliminationMode,
        float deficiency_wt = 0,
        float separation_wt = 0);

    Weight ObjectiveFn(Weight, Weight separation = 0) const;
    bool Classic() const;
    bool LBElimination() const;
    bool Mixed() const;

 protected:
    EliminationCriterion criterion_;
    EliminationMode mode_;
    float deficiency_wt_, separation_wt_;
};  // EliminationParameters

class EliminationAlgorithm {
 public:
    EliminationAlgorithm() = delete;
    EliminationAlgorithm(const EliminationAlgorithm&) = delete;
    void operator=(const EliminationAlgorithm&) = delete;

    explicit EliminationAlgorithm(const Graph*, const EliminationParameters*);
    virtual ~EliminationAlgorithm();

    virtual void Run();

    std::string str() const;

    const std::vector< size_t >& tie_count() const;
    const FillEdges* fill_edges() const;
    FillEdges* TakeFillEdges();
    const EliminationParameters* parameters() const;

 protected:
    void Init();
    void Elimination();
    void Eliminate(Vertex);
    VertexWeight ArgMin();
    VertexWeight TieBreak();
    Vertices MonotoneNbhd(Vertex);
    bool IsRemoved(Vertex);     // Check if vertex has been eliminated

    // Returns < fill_weight, separation_weight >
    std::pair< Weight, Weight > WeightOf(Vertex);

    const Graph* const G_;
    const EliminationParameters* const parameters_;

    EliminationOrder eo_;

    FillEdges* fill_edges_;
    VertexSet remaining_vertices_;

    std::vector< VertexWeight > ties_;
    std::vector< size_t > tie_count_;

    SeparatorBlocks* B_;
    std::map< VertexPair, Weight > unseparated_pairs_;
};  // EliminationAlgorithm

}  // namespace chordalg

#endif  // CHORDALGSRC_ELIMINATION_ALGORITHM_H_
