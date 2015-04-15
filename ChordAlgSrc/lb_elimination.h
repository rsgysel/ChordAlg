/*
 *  lb_elimination.h - computes a triangulation via lb-elimination
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

#ifndef INCLUDE_LB_ELIMINATION_H_
#define INCLUDE_LB_ELIMINATION_H_

#include <algorithm>
#include <map>
#include <set>
#include <utility>

#include "elimination_algorithm.h"
#include "intersection_graph.h"
#include "separator.h"
#include "vertices.h"

namespace chordalg {

struct LBCriterion : public EliminationCriterion {
 public:
    virtual ~LBCriterion() {}
    virtual Weight Calculate(Weight, Weight) const {
        return 0;
    }
 private:
    virtual Weight Calculate(Weight) const {
        return 0;
    }
};  // LBCriterion

struct RatioCriterion : public LBCriterion {
 public:
    virtual ~RatioCriterion() {}
    Weight Calculate(Weight deficiency, Weight separated) const {
        return deficiency / (1 + separated);
    }
};  // RatioCriterion

struct WSumCriterion : public LBCriterion {
 public:
    virtual ~WSumCriterion() {}
    WSumCriterion() : d_(1), s_(1) {}
    WSumCriterion(Weight d, Weight s) : d_(d), s_(s) {}

    Weight Calculate(Weight deficiency, Weight separated) const {
        return d_*deficiency - s_*separated;
    }
 private:
    Weight d_, s_;
};  // WSumCriterion

class LBElimination : public EliminationAlgorithm {
 public:
    LBElimination() = delete;
    LBElimination(const LBElimination&) = delete;
    void operator=(const LBElimination&) = delete;

    explicit LBElimination(const Graph*, const LBCriterion*);
    virtual ~LBElimination();

 protected:
    void Init();
    void Eliminate(Vertex);
    std::pair< Weight, Cost > WeightOf(Vertex);

    virtual std::pair< Weight, Cost > ObjectiveFunction(Weight, Weight);

    const LBCriterion* const f_;

    SeparatorBlocks B_;

    std::map< VertexPair, Weight > unseparated_pairs_;
};  // class LBElimination

}  // namespace chordalg

#endif  // INCLUDE_LB_ELIMINATION_H_
