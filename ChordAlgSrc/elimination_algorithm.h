/*
 *  elimination_algorithm.h - base class for vertex elimination algorithms
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

class EliminationParameters {
 public:

    EliminationParameters() = delete;
    EliminationParameters(const EliminationParameters&) = delete;
    void operator=(const EliminationParameters&) = delete;

    explicit EliminationParameters(EliminationCriterion, 
                                   bool atoms=false, 
                                   size_t runs=1, 
                                   float deficiency_wt=0, 
                                   float separation_wt=0);

    Weight ObjectiveFn(Weight deficiency, Weight separation=0) const;

    bool atoms() const {
        return atoms_;
    }

    size_t runs() const {
        return runs_;
    }
 protected:
    EliminationCriterion criterion_;
    bool atoms_;
    size_t runs_;
    float deficiency_wt_, separation_wt_;
};  // class EliminationParameters

class EliminationAlgorithm {
 public:
    EliminationAlgorithm() = delete;
    EliminationAlgorithm(const EliminationAlgorithm&) = delete;
    void operator=(const EliminationAlgorithm&) = delete;

    explicit EliminationAlgorithm(const Graph* G, const EliminationParameters*);
    virtual ~EliminationAlgorithm();

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

    AdjacencyLists* TriangNbhds() const;

 protected:
    virtual void Init();

    // Main Methods
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

    virtual void Eliminate(Vertex) = 0;
    virtual std::pair< Weight, Weight > WeightOf(Vertex) = 0;

    const Graph* const G_;
    const EliminationParameters* const parameters_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< size_t > alpha_inverse_;

    Weight fill_weight_;
    size_t fill_count_;  // # of fill edges added (monochromatic or not)
    std::vector< VertexSet > fill_neighbors_;
    std::vector< VertexPair > fill_edges_;
    VertexSet remaining_vertices_;

    std::vector< VertexWeight > ties_;
    std::vector< size_t > tie_count_;
};  // class EliminationAlgorithm

class ClassicElimination : public EliminationAlgorithm {
 public:
    explicit ClassicElimination(const Graph*, const EliminationParameters*);
    virtual ~ClassicElimination();

 private:
    void Eliminate(Vertex);
    std::pair< Weight, Weight > WeightOf(Vertex);
};  // class ClassicElimination

class LBElimination : public EliminationAlgorithm {
 public:
    LBElimination() = delete;
    LBElimination(const LBElimination&) = delete;
    void operator=(const LBElimination&) = delete;

    explicit LBElimination(const Graph*, const EliminationParameters*);
    virtual ~LBElimination();

 protected:
    void Init();
    void Eliminate(Vertex);
    std::pair< Weight, Weight > WeightOf(Vertex);

    SeparatorBlocks B_;
    std::map< VertexPair, Weight > unseparated_pairs_;
};  // class LBElimination

class MixedElimination : public LBElimination {
 public:
    MixedElimination() = delete;
    MixedElimination(const MixedElimination&) = delete;
    void operator=(const MixedElimination&) = delete;

    explicit MixedElimination(const Graph*, const EliminationParameters*);
    virtual ~MixedElimination();

 private:
    void Eliminate(Vertex);
    std::pair< Weight, Weight > WeightOf(Vertex);

    SeparatorBlocks B_;
};  // class MixedElimination

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ALGORITHM_H_
