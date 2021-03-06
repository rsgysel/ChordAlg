#include "ChordAlgSrc/triangulation.h"

#include <string>

#include "ChordAlgSrc/elimination.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/mcs.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Triangulation::Triangulation(const Graph* G) :
    Graph(*G),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, AdjacencyLists* nbhds) :
    Graph(nbhds),
    G_(G) {
    return;
}

Triangulation::Triangulation(const Graph* G, GraphFR* fr) :
    Graph(fr),
    G_(G) {
    return;
}

////////////////
// Triangulation

Triangulation* Triangulation::New(const Graph* G) {
    if (!Triangulation::IsTriangulated(*G)) {
        std::cerr << "Error in Triangulation::New:"
                  << " Graph is not chordal\n";
        delete G;
        exit(EXIT_FAILURE);
    }
    return new Triangulation(G);
}

Triangulation* Triangulation::New(const Graph* G, const EliminationOrder& eo) {
    return new Triangulation(G, Elimination::FillIn(*G, eo));
}

Triangulation* Triangulation::New(const Graph* G, const FillEdges& F) {
    return new Triangulation(G, F.FilledNbhds());
}

Triangulation* Triangulation::New(const Graph* G, const HeuristicRun& R) {
    return new Triangulation(G, R.fill_edges().FilledNbhds());
}

Triangulation* Triangulation::New(const Graph* G, const std::string& filename) {
    Triangulation* H = nullptr;
    GraphFile* file = GraphFile::New(filename);
    if (file->file_type() == FileType::ADJLIST ||
        file->file_type() == FileType::DIMACS) {
        GraphFR* file_reader = GraphFR::New(file);
        H = new Triangulation(G, file_reader);
        delete file_reader;
    } else if (file->file_type() == FileType::CHARACTERMATRIX ||
               file->file_type() == FileType::NEXUSMRP) {
        CellIntGraphFR* file_reader = CellIntGraphFR::New(file);
        H = new Triangulation(G, file_reader);
        delete file_reader;
    }
    return H;
}

VertexName Triangulation::name(Vertex v) const {
    return G_->name(v);
}

bool Triangulation::IsFillEdge(Vertex u, Vertex v) const {
    return IsEdge(u, v) && !G_->IsEdge(u, v);
}

bool Triangulation::IsFillEdge(VertexPair uv) const {
    return IsFillEdge(uv.first, uv.second);
}

bool Triangulation::IsTriangulated() const {
    return IsTriangulated(*this);
}

bool Triangulation::IsTriangulated(const Graph& G) {
    chordalg::EliminationOrder* eo = MCS::Run(G);
    bool result = Elimination::ZeroFill(G, *eo);
    delete eo;
    return result;
}

// Tests if a triangulation is minimal by successively removing
// a fill edge and checking chordality. See Rose et. al '76
// `Algorithmic Aspects of Vertex Elimination on Graphs'
// paper: http://epubs.siam.org/doi/abs/10.1137/0205021
//
bool Triangulation::IsMinimalTriangulation() const {
    if (!IsTriangulated()) {
        return false;
    }
    for (Vertex v : *this) {
        for (Vertex u : N(v)) {
            if (IsFillEdge(u, v)) {
                FillEdges* subfill = CopyFill();
                subfill->RemoveEdge(u, v);
                Triangulation Huv(G_, subfill->FilledNbhds());
                if (Huv.IsTriangulated()) {
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

const Graph& Triangulation::G() const {
    return *G_;
}

}  // namespace chordalg
