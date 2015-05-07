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

bool Triangulation::IsFillEdge(Vertex u, Vertex v) const {
    return HasEdge(u, v) && !G_->HasEdge(u, v);
}

bool Triangulation::IsFillEdge(VertexPair uv) const {
    return IsFillEdge(uv.first, uv.second);
}

bool Triangulation::IsChordal() const {
    chordalg::EliminationOrder* eo = MCS(*this);
    Triangulation* H = Triangulation::New(this, eo);
    bool result = IsIsomorphic(*H);
    delete H;
    delete eo;
    return result;
}

bool Triangulation::IsChordal(const Graph* G) {
    chordalg::EliminationOrder* eo = MCS(*G);
    Triangulation* H = Triangulation::New(G, eo);
    bool result = G->IsIsomorphic(*H);
    delete H;
    delete eo;
    return result;
}

// Tests if a triangulation is minimal by successively removing
// a fill edge and checking chordality. See Rose et. al '76
// `Algorithmic Aspects of Vertex Elimination on Graphs'
// paper: http://epubs.siam.org/doi/abs/10.1137/0205021
//
bool Triangulation::IsMinimalTriangulation() const {
    if (!IsChordal()) {
        return false;
    }
    for (Vertex v : *this) {
        for (Vertex u : N(v)) {
            if (IsFillEdge(u, v)) {
                FillEdges* subfill = CopyFill();
                subfill->RemoveEdge(u, v);
                Triangulation Huv(G_, subfill);
                if (Huv.IsChordal()) {
                    return false;
                }
                delete subfill;
            }
        }
    }
    return true;
}

FillEdges* Triangulation::CopyFill() const {
    FillEdges* F = new FillEdges(G_);
    for (Vertex v : *this) {
        for (Vertex u : N(v)) {
            F->AddEdge(u, v);
        }
    }
    return F;
}

}  // namespace chordalg
