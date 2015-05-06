#include "ChordAlgSrc/triangulation.h"

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/mcs.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Triangulation::Triangulation(const Graph* G, const EliminationAlgorithm* ea) :
    Graph(ea->TriangNbhds()),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, EliminationOrder* eo) :
    Graph(eo->TriangNbhds()),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, const HeuristicRun* R) :
    Graph(R->TriangNbhds()),
    G_(G) {
    return;
}

////////////////
// Triangulation

VertexName Triangulation::name(Vertex v) const {
    return G_->name(v);
}

bool Triangulation::IsChordal() const {
    chordalg::EliminationOrder* eo = MCS(*this);
    Triangulation H(this, eo);
    delete eo;
    return IsIsomorphic(H);
}

}  // namespace chordalg
