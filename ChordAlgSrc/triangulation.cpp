#include "ChordAlgSrc/triangulation.h"

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Triangulation::Triangulation(const Graph* G, const FillEdges* F) :
    Graph(F->TriangulationNbhds()),
    G_(G) {
    return;
}

////////////////
// Triangulation

Triangulation* Triangulation::New(const Graph* G, const EliminationOrder* eo) {
    FillEdges* F = eo->ComputeFill();
    Triangulation* H = new Triangulation(G, F);
    delete F;
    return H;
}

Triangulation* Triangulation::New(const Graph* G, const HeuristicRun* R) {
    Triangulation* H = new Triangulation(G, R->fill_edges());
    return H;
}

VertexName Triangulation::name(Vertex v) const {
    return G_->name(v);
}

bool Triangulation::IsChordal() const {
    chordalg::EliminationOrder* eo = MCS(*this);
    Triangulation* H = Triangulation::New(this, eo);
    bool result = IsIsomorphic(*H);
    delete H;
    delete eo;
    return result;
}

}  // namespace chordalg
