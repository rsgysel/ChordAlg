#include "ChordAlgSrc/vertices.h"

#include <algorithm>
#include <iterator>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lex_trie.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Vertices::Vertices() {
    return;
}

Vertices::Vertices(size_t n) : VertexVector(n, 0) {
    return;
}
Vertices::Vertices(FiniteSet S) : VertexVector(S.begin(), S.end()) {
    return;
}

Vertices::Vertices(VertexList V) : VertexVector(V.begin(), V.end()) {
    return;
}

Vertices::Vertices(VertexSet V) : VertexVector(V.begin(), V.end()) {
    return;
}

Vertices::Vertices(VertexVector V) : VertexVector(V.begin(), V.end()) {
    return;
}

Vertices::Vertices(std::initializer_list<Vertex> V) :
    VertexVector(V.begin(), V.end()) {
    return;
}

GraphVertexIterator::GraphVertexIterator(const Graph* G) : G_(G), v_(0) {
    return;
}

GraphVertexIterator::GraphVertexIterator(const Graph* G, Vertex v) :
    G_(G),
    v_(v) {
    return;
}

GraphVertices::GraphVertices(const Graph* G, size_t order) :
    Vertices(),
    G_(G),
    order_(order) {
    return;
}

VertexPairsIterator::VertexPairsIterator(
    const Vertices* V,
    Vertex v1,
    Vertex v2) :
    V_(V),
    v1_(v1),
    v2_(v1) {
    if (V->size() == 1) {
        v1_ = v2_ = V->size();
    } else if (v1 != v2) {
        v2_++;
    }
    return;
}

VertexPairs::VertexPairs(Vertices V) : V_(V), begin_(0), end_(V_.size()) {
    return;
}

///////////
// Vertices

void Vertices::sort() {
    std::sort(begin(), end());
}

void Vertices::merge(Vertices U, Vertices W) {
    clear();
    reserve(U.size() + W.size());
    std::merge(U.begin(), U.end(),
               W.begin(), W.end(),
               std::back_inserter(*this));
    return;
}

//////////////////////
// GraphVertexIterator

void GraphVertexIterator::operator++() {
    ++v_;
}

Vertex GraphVertexIterator::operator*() const {
    return v_;
}

bool GraphVertexIterator::operator!=(const GraphVertexIterator& other) const {
    return !(*this == other);
}

bool GraphVertexIterator::operator==(const GraphVertexIterator& other) const {
    return (G_ == other.G_) && (v_ == other.v_);
}

////////////////
// GraphVertices

GraphVertexIterator GraphVertices::begin() const {
    return GraphVertexIterator(G_);
}
GraphVertexIterator GraphVertices::end() const {
    return GraphVertexIterator(G_, order_);
}

//////////////////////
// VertexPairsIterator

void VertexPairsIterator::operator++() {
    size_t n = V_->size();
    if (n == 0 || v1_ == n) {
        return;
    }
    if (v2_ == n - 1) {
        ++v1_;
        if (v1_ == n - 1) {
            v1_ = v2_ = n;
        } else {
            v2_ = v1_ + 1;
        }
    } else {
        ++v2_;
    }
    return;
}

bool VertexPairsIterator::operator==(const VertexPairsIterator& other) const {
    return (V_ == other.V_) && (v1_ == other.v1_) && (v2_ == other.v2_);
}

bool VertexPairsIterator::operator!=(const VertexPairsIterator& other) const {
    return !(*this == other);
}

VertexPair VertexPairsIterator::operator*() {
    return VertexPair((*V_)[v1_], (*V_)[v2_]);
}

//////////////
// VertexPairs

VertexPairsIterator VertexPairs::begin() const {
    return VertexPairsIterator(&V_, begin_, end_);
}

VertexPairsIterator VertexPairs::end() const {
    return VertexPairsIterator(&V_, end_, end_);
}

}  // namespace chordalg
