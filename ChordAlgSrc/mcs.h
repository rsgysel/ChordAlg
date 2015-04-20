/*
 *  mcs.h - mcs algorithm & mcs queue
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
#include "vertices.h"

namespace chordalg {

EliminationOrder* MCS(const Graph& G);
CliqueTree* MCSCliqueTree(const Graph& G);

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
