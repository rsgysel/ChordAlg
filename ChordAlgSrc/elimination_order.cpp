#include "elimination_order.h"

#include <algorithm>
#include <utility>

namespace chordalg {

EliminationOrder::EliminationOrder(const Graph* G) :
                                   G_(G),
                                   alpha_(G_->order()),
                                   alpha_inverse_(G_->order()),
                                   fill_count_(kUnfilled()) {
    Init();
    return;
}

EliminationOrder::~EliminationOrder() {
    return;
}

void EliminationOrder::Init() {
    int i = 0;
    for (Vertex v : *G_) {
        alpha_[i] = v;
        alpha_inverse_[v] = i;
        ++i;
    }
    return;
}

void EliminationOrder::Emplace(Vertex v, int i) {
    Swap(i, alpha_inverse_[v]);
    return;
}

void EliminationOrder::Swap(int i, int j) {
    Vertex  vi = alpha_[i],
            vj = alpha_[j];
    std::swap(alpha_[i], alpha_[j]);
    std::swap(alpha_inverse_[vi], alpha_inverse_[vj]);
    return;
}

// Produces (and passes ownership of) adjacency lists
// form triagnulation neighborhoods
AdjacencyLists* EliminationOrder::TriangNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(G_->order());
    if (fill_count_ == 0) {
        for (Vertex v : *G_) {
            a_lists->operator[](v).reserve(triangulation_nbhds_[v].size());
            for (Vertex u : G_->N(v)) {
                a_lists->operator[](v).add(u);
            }
        }
    } else {
        for (Vertex v : *G_) {
            a_lists->operator[](v).reserve(triangulation_nbhds_[v].size());
            for (Vertex u : triangulation_nbhds_[v]) {
                a_lists->operator[](v).add(u);
            }
        }
    }
    return a_lists;
}

// Tarjan and Yannakakis' algorithm to compute fill edges from an elimination
// order, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test
// chordality of graphs, test acyclicity of hypergraphs, and selectively
// reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
int EliminationOrder::ComputeFill() {
    if (fill_count_ != kUnfilled()) {
        return fill_count_;
    }
    fill_count_ = 0;
    triangulation_nbhds_.resize(G_->order());
    VertexVector follower;
    follower.resize(G_->order());
    VertexVector index;
    index.resize(G_->order());
    for (size_t i = 0; i < G_->order(); ++i) {
        Vertex w = VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for (Vertex v : LNbhd(w)) {
            Vertex x = v;
            while (index[x] < i) {
                index[x] = i;
                ++fill_count_;
                triangulation_nbhds_[x].insert(w);
                triangulation_nbhds_[w].insert(x);
                x = follower[x];
            }
            if (follower[x] == x) {
                follower[x] = w;
            }
        }
    }
    fill_count_ -= G_->size();
    return fill_count_;
}

// Tarjan and Yannakakis' algorithm to check if an elimination order is
// perfect, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test
// chordality of graphs, test acyclicity of hypergraphs, and selectively
// reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
bool EliminationOrder::ZeroFill() const {
    if (fill_count_ != kUnfilled()) {
        return fill_count_ == 0;
    }
    VertexVector follower;
    follower.resize(G_->order());
    VertexVector index;
    index.resize(G_->order());
    for (size_t i = 0; i < G_->order(); ++i) {
        Vertex w = VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for (Vertex v : LNbhd(w)) {
            index[v] = i;
            if (follower[v] == v) {
                follower[v] = w;
            }
        }
        for (Vertex v : LNbhd(w)) {
            if (index[follower[v]] < i) {
                return false;
            }
        }
    }
    return true;
}

Vertices EliminationOrder::LNbhd(Vertex v) const {
    Vertices L_N;
    for (Vertex u : G_->N(v)) {
        if (Before(u, v)) {
            L_N.add(u);
        }
    }
    if (!triangulation_nbhds_.empty()) {
        for (Vertex u : triangulation_nbhds_[v]) {
            if (Before(u, v)) {
                L_N.add(u);
            }
        }
    }
    return L_N;
}

Vertices EliminationOrder::RNbhd(Vertex v) const {
    Vertices R_N;
    for (Vertex u : G_->N(v)) {
        if (Before(v, u)) {
            R_N.add(u);
        }
    }
    if (!triangulation_nbhds_.empty()) {
        for (Vertex u : triangulation_nbhds_[v]) {
            if (Before(v, u)) {
                R_N.add(u);
            }
        }
    }
    return R_N;
}

void EliminationOrder::SetOrder(VertexVector pi) {
    if (pi.size() != G_->order()) {
        std::cerr << "Error in SetOrder: elimination order size and graph ";
        std::cerr << "order does not match" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < pi.size(); ++i) {
        Emplace(pi[i], i);
    }
    return;
}

void EliminationOrder::PrettyPrint() const {
    std::cout << "elimination order:\t";
    for (Vertex v : alpha_) {
        std::cout << G_->name(v) << " ";
    }
    std::cout << std::endl;
    return;
}

}  // namespace chordalg
