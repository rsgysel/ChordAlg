#include "mixed_elimination.h"

#include <utility>

namespace chordalg {

MixedElimination::MixedElimination(const Graph* G,
                                   const LBCriterion* f) :
                                   LBElimination(G, f),
                                   B_(G) {
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
            unseparated_pairs_.erase(uv);
        }
    }
    return;
}

std::pair< Weight, Cost > MixedElimination::WeightOf(Vertex v) {
    Weight c = 0;
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        if (!IsEdge(p)) {
            c += G_->FillCost(p);
        }
    }
    // Don't need separation statistics
    return std::pair<Weight, Cost>(c, c);
}

}  // namespace chordalg
