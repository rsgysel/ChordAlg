#include "classic_elimination.h"

#include <utility>

namespace chordalg {

ClassicElimination::ClassicElimination(const Graph* G,
                                       const ClassicCriterion* f) :
                                       EliminationAlgorithm(G),
                                       f_(f) {
    EliminationAlgorithm::Init();
    return;
}

ClassicElimination::~ClassicElimination() {
    delete f_;
    return;
}

void ClassicElimination::Eliminate(Vertex v) {
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        AddEdge(p);
    }
    return;
}

std::pair< Weight, Cost > ClassicElimination::WeightOf(Vertex v) {
    Cost c = 0;
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        if (!IsEdge(p)) {
            c += G_->FillCost(p);
        }
    }
    return std::pair< Weight, Cost >(f_->Calculate(c), c);
}

}  // namespace chordalg
