#ifndef CHORDALGSRC_TRIANGULATION_H_
#define CHORDALGSRC_TRIANGULATION_H_

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/heuristic_run.h"

namespace chordalg {

class Triangulation : public Graph {
 public:
    Triangulation() = delete;
    Triangulation(const Triangulation&) = delete;
    void operator=(const Triangulation&) = delete;

    explicit Triangulation(const Graph*, const EliminationAlgorithm*);
    explicit Triangulation(const Graph*, EliminationOrder*);
    explicit Triangulation(const Graph*, const HeuristicRun*);

    VertexName name(Vertex v) const {
        return G_->name(v);
    }
    bool IsChordal() const;
 protected:
    const Graph* const G_;
};  // Triangulation

}  // namespace chordalg

#endif  // CHORDALGSRC_TRIANGULATION_H_
