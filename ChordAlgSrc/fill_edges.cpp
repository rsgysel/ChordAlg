#include "ChordAlgSrc/fill_edges.h"

#include <algorithm>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

FillEdges::FillEdges(const Graph* G) :
    std::vector< VertexSet >(G->order(), VertexSet()),
    G_(G),
    fill_count_(0),
    fill_weight_(0),
    neighborhoods_(new AdjacencyLists(G_->neighborhoods())) {
    return;
}

////////////
// FillEdges

const Vertices& FillEdges::N(Vertex v) const {
    return (*neighborhoods_)[v];
}

bool FillEdges::IsEdge(VertexPair uv) const {
    return IsEdge(uv.first, uv.second);
}

bool FillEdges::IsEdge(Vertex u, Vertex v) const {
    return G_->IsEdge(u, v) || IsFillEdge(u, v);
}

bool FillEdges::IsFillEdge(VertexPair uv) const {
    return IsFillEdge(uv.first, uv.second);
}

bool FillEdges::IsFillEdge(Vertex u, Vertex v) const {
    return (*this)[u].find(v) != (*this)[u].end();
}

bool FillEdges::IsClique(const Vertices& U) const {
    for (VertexPair uv : VertexPairs(U)) {
        if (!IsEdge(uv)) {
            return false;
        }
    }
    return true;
}

bool FillEdges::AddEdge(VertexPair uv) {
    return AddEdge(uv.first, uv.second);
}

bool FillEdges::AddEdge(Vertex u, Vertex v) {
    if (IsEdge(u, v)) {
        return false;
    } else {
        (*this)[u].insert(v);
        (*neighborhoods_)[u].push_back(v);
        (*this)[v].insert(u);
        (*neighborhoods_)[v].push_back(u);
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

void FillEdges::Saturate(const Vertices& U) {
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

AdjacencyLists* FillEdges::FilledNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists(G_->neighborhoods());
    for (Vertex v : *G_) {
        for (Vertex u : (*this)[v]) {
            (*a_lists)[v].push_back(u);
        }
        std::sort((*a_lists)[v].begin(), (*a_lists)[v].end());
    }
    return a_lists;
}

}  // namespace chordalg
