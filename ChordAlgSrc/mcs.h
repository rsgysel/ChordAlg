/*
 *  mcs.h - mcs algorithm & mcs queue
 */


#ifndef CHORDALGSRC_MCS_H_
#define CHORDALGSRC_MCS_H_

#include <set>
#include <vector>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/tree_representation.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

EliminationOrder* MCS(const Graph* G);
CliqueTree* MCSCliqueTree(const Graph* G);

class MCSQueue {
 public:
    explicit MCSQueue(size_t order);

    size_t max_weight() const;
    Vertex Pop();
    void Increment(Vertex v);

 private:
    static int kDeletedVertex();

    size_t order_;
    size_t max_weight_;
    size_t remaining_vertices_;
    std::vector< std::set<Vertex> > queue_;
    std::vector< int > weight_;
};  // MCSQueue

}  // namespace chordalg

#endif  // CHORDALGSRC_MCS_H_
