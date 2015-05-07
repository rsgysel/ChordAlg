#include "ChordAlgSrc/fill_edges.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

FillEdges::FillEdges(const Graph* G) :
    std::vector< VertexSet >(G->order(), VertexSet()),
    G_(G),
    fill_count_(0),
    fill_weight_(0) {
    return;
}

////////////
// FillEdges

bool FillEdges::IsEdge(VertexPair uv) const {
    return IsEdge(uv.first, uv.second);
}

bool FillEdges::IsEdge(Vertex u, Vertex v) const {
    return G_->HasEdge(u, v) || IsFillEdge(u, v);
}

bool FillEdges::IsFillEdge(VertexPair uv) const {
    return IsFillEdge(uv.first, uv.second);
}

bool FillEdges::IsFillEdge(Vertex u, Vertex v) const {
    return (*this)[u].find(v) != (*this)[u].end();
}

bool FillEdges::AddEdge(VertexPair uv) {
    return AddEdge(uv.first, uv.second);
}

bool FillEdges::AddEdge(Vertex u, Vertex v) {
    if (IsEdge(u, v)) {
        return false;
    } else {
        (*this)[u].insert(v);
        (*this)[v].insert(u);
        fill_count_++;
        fill_weight_ += G_->FillCount(u, v);
        return true;
    }
}

void FillEdges::RemoveEdge(VertexPair uv) {
    RemoveEdge(uv.first, uv.second);
    return;
}

void FillEdges::RemoveEdge(Vertex u, Vertex v) {
    (*this)[u].erase(v);
    (*this)[v].erase(u);
    return;
}

void FillEdges::Saturate(Vertices U) {
    for (VertexPair uv : VertexPairs(U)) {
        AddEdge(uv);
    }
    return;
}

size_t FillEdges::fill_count() const {
    return fill_count_;
}

Weight FillEdges::fill_weight() const {
    return fill_weight_;
}

AdjacencyLists* FillEdges::TriangulationNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists(G_->neighbors());
    for (Vertex v: *G_) {
        for (Vertex u : (*this)[v]) {
            (*a_lists)[v].push_back(u);
        }
    }
    return a_lists;
}

}  // namespace chordalg
