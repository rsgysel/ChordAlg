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

}  // namespace chordalg
