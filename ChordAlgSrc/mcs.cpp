#include "mcs.h"

#include <algorithm>
#include <list>
#include <utility>
#include <vector>

namespace chordalg {

EliminationOrder* MCS(const Graph& G) {
    MCSQueue mcs_q(G.order());
    EliminationOrder* eo = new EliminationOrder(&G);
    for (size_t i = G.order() ; i > 0 ; i--) {
        Vertex v = mcs_q.Pop();
        eo->Emplace(v, i - 1);
        if (i != 1) {
            for (Vertex u : G.N(v)) {
                mcs_q.Increment(u);
            }
        }
    }
    return eo;
}

CliqueTree* MCSCliqueTree(const Graph& G) {
    MCSQueue mcs_q(G.order());
    EliminationOrder eo(&G);
    std::list< std::pair<size_t, size_t> > ct_edges;
    size_t s = 0;
    VertexList Ks;
    std::list<size_t> Ks_indices;
    size_t prev_card = 0;
    bool init_prev_card = true;
    std::vector<size_t> clique;
    clique.resize(G.order());
    std::vector< Vertices > clique_map;
    for (size_t i = G.order(); i > 0; i--) {
        // Get max weight vertex and update others
        size_t new_card = mcs_q.max_weight();
        Vertex v = mcs_q.Pop();
        eo.Emplace(v, i - 1);
        if (i != 1) {
            for (Vertex u : G.N(v)) {
                mcs_q.Increment(u);
            }
        }
        // Check for maxclique construction
        if (new_card <= prev_card || init_prev_card) {
            init_prev_card = false;
            clique_map.push_back(Vertices(Ks));
            Ks.clear();
            Ks_indices.clear();
            ++s;
            for (Vertex u : eo.RNbhd(v)) {
                Ks.push_back(u);
                Ks_indices.push_back(eo.PositionOf(u));
            }
            if (new_card != 0) {
                size_t k = *std::min_element(Ks_indices.begin(),
                                             Ks_indices.end());
                size_t p = clique[eo.VertexAt(k)];
                ct_edges.push_back(std::make_pair(s, p));
            }
        }
        clique[v] = s;
        Ks.push_back(v);
        Ks_indices.push_back(i);
        prev_card = new_card;
    }
    if (!Ks.empty()) {
        clique_map.push_back(Vertices(Ks));
    }
    AdjacencyLists* E = new AdjacencyLists;
    E->resize(s + 1);
    for (std::pair<int, int> e : ct_edges) {
        (*E)[e.first].add(e.second);
        (*E)[e.second].add(e.first);
    }
    CliqueTree* tr = new CliqueTree(E, &G, clique_map);
    return tr;
}

MCSQueue::MCSQueue(size_t order) :
    order_(order),
    max_weight_(0),
    remaining_vertices_(order) {
    queue_.resize(order_ + 1);
    weight_.resize(order_);
    for (Vertex v = 0; v < order_; ++v) {
        queue_[0].insert(v);
        weight_[v] = 0;
    }
}

Vertex MCSQueue::Pop() {
    if (remaining_vertices_ == 0) {
        std::cerr << "Error in MCSQueue: pop requested on empty queue";
        std::cerr << std::endl;
        exit(EXIT_FAILURE);
    }
    Vertex v = *(queue_[max_weight_].begin());
    queue_[max_weight_].erase(v);
    weight_[v] = kDeletedVertex();
    --remaining_vertices_;
    while (remaining_vertices_ > 0 && queue_[max_weight_].empty()) {
        --max_weight_;
    }
    return v;
}

void MCSQueue::Increment(Vertex v) {
    if (weight_[v] != kDeletedVertex()) {
        queue_[weight_[v]].erase(v);
        weight_[v]++;
        queue_[weight_[v]].insert(v);
        if (weight_[v] > max_weight_) {
            max_weight_++;
        }
    }
    return;
}

}  // namespace chordalg
