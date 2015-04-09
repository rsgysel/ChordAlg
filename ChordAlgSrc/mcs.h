/*
 *  mcs.h - queue structure for the mcs algorithm
 *  Copyright (C) 2014 Rob Gysel
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


#ifndef INCLUDE_MCS_H_
#define INCLUDE_MCS_H_

#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "elimination_order.h"
#include "graph.h"
#include "tree_representation.h"
#include "vertex_utilities.h"

namespace chordalg {

EliminationOrder* MCS(Graph& G);
CliqueTree* MCSCliqueTree(Graph& G);

class MCSQueue {
 public:
    explicit MCSQueue(size_t order);
    ~MCSQueue() {}

    int max_weight() {
        return max_weight_;
    }

    Vertex Pop();
    void Increment(Vertex v);

 private:
    static int kDeletedVertex() {
        return -1;
    }

    size_t order_;
    int max_weight_;
    size_t remaining_vertices_;
    std::vector< std::set<Vertex> > queue_;
    std::vector< int > weight_;
};  // class MCSQueue

}  // namespace chordalg

#endif  // INCLUDE_MCS_H_
