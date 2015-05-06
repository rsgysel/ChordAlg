#include "ChordAlgSrc/triangulation.h"

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/mcs.h"

namespace chordalg {

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

bool Triangulation::IsChordal() const {
    chordalg::EliminationOrder* eo = MCS(*this);
    Triangulation H(this, eo);
    delete eo;
    return IsIsomorphic(H);
}

}  // namespace chordalg
