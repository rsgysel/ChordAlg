#include "vertices.h"

namespace chordalg {

void Vertices::merge(Vertices U, Vertices W) {
    V_->clear();
    V_->reserve(U.size() + W.size());
    std::merge(U.begin(), U.end(),
               W.begin(), W.end(),
               std::back_inserter(*V_));
    return;
}

////////////// VertexPairs
////////////// ctor
//

VertexPairsIterator::VertexPairsIterator(const Vertices* V, Vertex v1, Vertex v2) :
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

////////////// Iterator
//

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

}  // namespace chordalg
