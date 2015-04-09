#include "mixed_elimination.h"

#include <utility>

namespace chordalg {

MixedElimination::MixedElimination(const ColoredIntersectionGraph* H,
                                   const LBCriterion* f) :
                                   LBElimination(H, f),
                                   H_(H),
                                   B_(H) {
    LBElimination::Init();
    return;
}

MixedElimination::~MixedElimination() {
    return;
}

void MixedElimination::Eliminate(Vertex v) {
    Vertices S = MonotoneNbhd(v);
    S.add(v);
    B_.Separate(S, fill_neighbors_);
    for (Block B : B_) {
        for (VertexPair uv : VertexPairs(B.NC())) {
            AddEdge(uv);
            unseparated_monochromatic_pairs_.erase(uv);
        }
    }
    return;
}

std::pair< Weight, Cost > MixedElimination::WeightOf(Vertex v) {
    Weight wt = 0;
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        if (!IsEdge(p)) {
            wt += H_->CommonColorCount(p.first, p.second);
        }
    }
    // Don't need separation statistics
    return std::pair<Weight, Cost>(wt, wt);
}

}  // namespace chordalg
