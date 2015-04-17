#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include "graph.h"
#include "elimination_algorithm.h"
#include "elimination_order.h"

namespace chordalg {

class Triangulation : public Graph {
 public:
    Triangulation() = delete;
    Triangulation(const Triangulation&) = delete;
    void operator=(const Triangulation&) = delete;

    Triangulation(const Graph*, const EliminationAlgorithm*);
    Triangulation(const Graph*, const EliminationOrder*);

    VertexName name(Vertex v) const {
        return G_->name(v);
    }
 protected:
    const Graph* const G_;
};  // Triangulation

}  // namespace chordalg

#endif  // TRIANGULATION_H_
