/*
 *  elimination_order.h - data structure representing an elimination
 *  ordering of a graph
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

/*
 *  This data structure provides functionality used by triangulation
 *  algorithms based on classical elimination or LB-elimination.
 */

#ifndef INCLUDE_ELIMINATION_ORDER_H_
#define INCLUDE_ELIMINATION_ORDER_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "file_reader.h"
#include "graph.h"
#include "vertices.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

class EliminationOrder {
 public:
    EliminationOrder() = delete;
    EliminationOrder(const EliminationOrder&) = delete;
    void operator=(const EliminationOrder&) = delete;

    explicit EliminationOrder(const Graph*);
    ~EliminationOrder();
    void Init();

    size_t ComputeFill();
    void Emplace(Vertex v, int i);
    void Swap(int i, int j);
    AdjacencyLists* TriangNbhds() const;

    std::string str() const;
    bool ZeroFill() const;

    bool Before(Vertex u, Vertex v) const {
        return alpha_inverse_[u] < alpha_inverse_[v];
    }
    int PositionOf(Vertex v) const {
        return alpha_inverse_[v];
    }
    Vertex VertexAt(int i) const {
        return alpha_[i];
    }

    // neighbors of v ``left" (before) v. Unsorted.
    Vertices LNbhd(Vertex v) const;
    // neighbors of v ``right" (after) v. Unsorted.
    Vertices RNbhd(Vertex v) const;

    int fill_count() const {
        return fill_count_;
    }
    int size() const {
        return alpha_.size();
    }
    const Graph* G() const {
        return G_;
    }

    // Mutators
    void SetOrder(VertexVector pi);
    void SetPosition(Vertex v, int i) {
        alpha_inverse_[v] = i;
    }
    void SetVertex(int i, Vertex v) {
        alpha_[i] = v;
    }

 private:
    const Graph* const G_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< int > alpha_inverse_;

    int fill_count_;  // # of fill edges added (monochromatic or not)

    std::vector < VertexSet > triangulation_nbhds_;

    static int kUnfilled() {
        return -1;
    }
};  // class EliminationOrder

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ORDER_H_
