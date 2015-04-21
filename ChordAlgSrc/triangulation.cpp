#include "triangulation.h"

namespace chordalg {

Triangulation::Triangulation(const Graph* G, const EliminationAlgorithm* ea) :
    Graph(ea->TriangNbhds()),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, const EliminationOrder* eo) :
    Graph(eo->TriangNbhds()),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, const HeuristicRun* R) :
    Graph(R->TriangNbhds()),
    G_(G) {
    return;
}

bool Triangulation::IsChordal() const {
    chordalg::EliminationOrder* eo = MCS(*this);
    eo->ComputeFill();
    Triangulation H(this, eo);
    delete eo;
    return IsIsomorphic(H);
}

}  // namespace chordalg
