#include "ChordAlgSrc/vertices.h"

#include <algorithm>
#include <iterator>
#include <string>

#include "ChordAlgSrc/graph.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Vertices::Vertices() {
    return;
}

Vertices::Vertices(size_t n) : V_(n, 0) {
    return;
}

Vertices::Vertices(const VertexList& V) : V_(V.begin(), V.end()) {
    return;
}

Vertices::Vertices(const VertexSet& V) : V_(V.begin(), V.end()) {
    return;
}

Vertices::Vertices(const VertexVector& V) : V_(V) {
    return;
}

Vertices::Vertices(const std::initializer_list<Vertex>& V) : V_(V) {
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

VertexVector::const_iterator Vertices::begin() const {
    return V_.begin();
}

VertexVector::iterator Vertices::begin() {
    return V_.begin();
}

VertexVector::const_iterator Vertices::end() const {
    return V_.end();
}

VertexVector::iterator Vertices::end() {
    return V_.end();
}

void Vertices::operator=(const Vertices& other) {
    V_ = other.V_;
}

const Vertex& Vertices::operator[](size_t i) const {
    return V_[i];
}

Vertex& Vertices::operator[](size_t i) {
    return V_[i];
}

const Vertex& Vertices::back() const {
    return V_.back();
}

Vertex& Vertices::back() {
    return V_.back();
}

void Vertices::push_back(Vertex v) {
    V_.push_back(v);
    return;
}

void Vertices::pop_back() {
    V_.pop_back();
}

void Vertices::clear() {
    V_.clear();
}

void Vertices::reserve(size_t n) {
    V_.reserve(n);
    return;
}

size_t Vertices::size() const {
    return V_.size();
}

bool Vertices::empty() const {
    return V_.empty();
}

const VertexVector& Vertices::V() const {
    return V_;
}

void Vertices::Sort() {
    std::sort(begin(), end());
}

void Vertices::SetIntersection(const Vertices& U, const Vertices& W) {
    clear();
    reserve(std::min(U.size(), W.size()));
    if (U.empty() || W.empty()) {
        return;
    } else {
        std::set_intersection(U.begin(), U.end(),
                              W.begin(), W.end(),
                              std::back_inserter(V_));
    }
    return;
}

void Vertices::SetUnion(const Vertices& U, const Vertices& W) {
    clear();
    reserve(U.size() + W.size());
    if (U.empty()) {
        *this = W;
    } else if (W.empty()) {
        *this = U;
    } else {
        std::set_union(U.begin(), U.end(),
                       W.begin(), W.end(),
                       std::back_inserter(V_));
    }
    return;
}

std::string Vertices::str() const {
    std::string result;
    if (empty()) {
        return result;
    }
    for (auto v : *this) {
        result += std::to_string(v) + std::string(" ");
    }
    result.pop_back();
    return result;
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
