#ifndef PERFECT_ELIMINATION_ORDER_H
#define PERFECT_ELIMINATION_ORDER_H

#include "elimination_order.h"
#include "graph.h"
#include "vertex_utilities.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <utility>

namespace chordalg{

EliminationOrder MCS(Graph& G);

class MCSQueue {

public:
    MCSQueue(int order);
    ~MCSQueue() {};
    Vertex Pop();
    void Increment(Vertex v);

private:
    static int kDeletedVertex(){ return -1; }

    int order_;
    int max_weight_;
    int remaining_vertices_;
    std::vector< std::set<Vertex> > queue_;
    std::vector< int > weight_;

}; // class MCSQueue

} // namespace chordalg

#endif // PERFECT_ELIMINATION_ORDER_H
