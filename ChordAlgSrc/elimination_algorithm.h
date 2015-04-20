/*
 *  elimination_algorithm.h - base class for vertex elimination algorithms
 *  Copyright (C) 2013 Rob Gysel
 */

#ifndef INCLUDE_ELIMINATION_ALGORITHM_H_
#define INCLUDE_ELIMINATION_ALGORITHM_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "atoms.h"
#include "elimination_order.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "graph.h"
#include "separator.h"
#include "vertices.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

enum class EliminationCriterion {
    DEFICIENCY, // #deficient pairs needed to saturate nbhd
    RATIO,      // #separated pairs / #deficient pairs
    WSUM        // #separated pairs - d #deficient pairs
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

    explicit EliminationParameters(EliminationCriterion,
                                   EliminationMode,
                                   float deficiency_wt=0, 
                                   float separation_wt=0);

    Weight ObjectiveFn(Weight deficiency, Weight separation=0) const;
    bool Classic() const;
    bool LBElimination() const;
    bool Mixed() const;

 protected:
    EliminationCriterion criterion_;
    EliminationMode mode_;
    float deficiency_wt_, separation_wt_;

};  // class EliminationParameters

class EliminationAlgorithm {
 public:
    EliminationAlgorithm() = delete;
    EliminationAlgorithm(const EliminationAlgorithm&) = delete;
    void operator=(const EliminationAlgorithm&) = delete;

    explicit EliminationAlgorithm(const Graph* G, const EliminationParameters*);
    virtual ~EliminationAlgorithm();

    virtual void Run();

    std::string str() const;

    Weight fill_weight() const {
        return fill_weight_;
    }
    size_t fill_count() const {
        return fill_count_;
    }
    const std::vector< size_t >& tie_count() const {
        return tie_count_;
    }
    const std::vector< VertexSet >& fill_neighbors() const {
        return fill_neighbors_;
    }
    const std::vector< VertexPair >& fill_edges() const {
        return fill_edges_;
    }
    const EliminationParameters* parameters() const {
        return parameters_;
    }

    AdjacencyLists* TriangNbhds() const;

 protected:
    // Main Methods
    void Init();
    void Elimination();
    VertexWeight ArgMin();
    virtual VertexWeight TieBreak();  // base method breaks ties uniformly
    Vertices MonotoneNbhd(Vertex);

    // Helpers
    void AddEdge(VertexPair);
    bool IsEdge(VertexPair);
    bool IsFillEdge(VertexPair);
    bool IsRemoved(Vertex);
    void Saturate(Vertices);

    virtual void Eliminate(Vertex);
    virtual std::pair< Weight, Weight > WeightOf(Vertex);

    const Graph* const G_;
    const EliminationParameters* const parameters_;

    EliminationOrder eo_;

    Weight fill_weight_;
    size_t fill_count_;  // # of fill edges added (monochromatic or not)
    std::vector< VertexSet > fill_neighbors_;
    std::vector< VertexPair > fill_edges_;
    VertexSet remaining_vertices_;

    std::vector< VertexWeight > ties_;
    std::vector< size_t > tie_count_;

    SeparatorBlocks* B_;
    std::map< VertexPair, Weight > unseparated_pairs_;
};  // class EliminationAlgorithm

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ALGORITHM_H_
