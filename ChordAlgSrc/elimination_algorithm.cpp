#include "elimination_algorithm.h"

#include <algorithm>
#include <utility>

namespace chordalg {

EliminationAlgorithm::EliminationAlgorithm(const Graph* G) :
                                           G_(G),
                                           alpha_(G_->order()),
                                           alpha_inverse_(),
                                           fill_cost_(0),
                                           fill_count_(0),
                                           fill_neighbors_(),
                                           remaining_vertices_(),
                                           ties_(),
                                           tie_count_() {
    srand(time(nullptr));
    return;
}

EliminationAlgorithm::~EliminationAlgorithm() {
    return;
}

void EliminationAlgorithm::Init() {
    int n = G_->order();

    alpha_inverse_.resize(n);
    fill_neighbors_.resize(n);
    tie_count_.resize(n);
    for (Vertex v : *G_) {
        remaining_vertices_.insert(v);
    }
    Elimination();
    return;
}

void EliminationAlgorithm::Elimination() {
    for (size_t i = 0; i < G_->order(); ++i) {
        VertexCost vc = ArgMin();
        fill_cost_ += vc.second;
        Vertex v = vc.first;

        alpha_[i] = v;
        alpha_inverse_[v] = i;

        tie_count_[i] = ties_.size();
        ties_.clear();

        remaining_vertices_.erase(v);
        Eliminate(v);
    }
    return;
}

void EliminationAlgorithm::AddEdge(VertexPair p) {
    if (!IsEdge(p)) {
        fill_neighbors_[p.first].insert(p.second);
        fill_neighbors_[p.second].insert(p.first);
        ++fill_count_;
    }
    return;
}

bool EliminationAlgorithm::IsEdge(VertexPair p) {
    return G_->HasEdge (p) || IsFillEdge(p);
}

bool EliminationAlgorithm::IsFillEdge(VertexPair p) {
    return fill_neighbors_[p.first].find(p.second) !=
           fill_neighbors_[p.first].end();
}

bool EliminationAlgorithm::IsRemoved(Vertex v) {
    return remaining_vertices_.find(v) == remaining_vertices_.end();
}

void EliminationAlgorithm::Saturate(Vertices U) {
    for (VertexPair p : VertexPairs(U)) {
        if (!IsEdge(p)) {
            Vertex u = p.first, v = p.second;
            fill_neighbors_[u].insert(v);
            fill_neighbors_[v].insert(u);
        }
    }
    return;
}

VertexCost EliminationAlgorithm::ArgMin() {
    Weight min = MAX_WEIGHT;
    for (Vertex v : remaining_vertices_) {
        std::pair< Weight, Cost > wc = WeightOf(v);
        Weight wt = wc.first;
        Cost c = wc.second;

        if (wt < min) {
            min = wt;
            ties_.clear();
            ties_.push_back(VertexCost(v, c));
        } else if (wt == min) {
            ties_.push_back(VertexCost(v, c));
        }
    }
    return TieBreak();
}

VertexCost EliminationAlgorithm::TieBreak() {
    int i = rand() % ties_.size();
    return ties_[i];
}

Vertices EliminationAlgorithm::MonotoneNbhd(Vertex v) {
    Vertices N_alpha;
    for (Vertex u : G_->N(v)) {
        if (!IsRemoved(u)) {
            N_alpha.add(u);
        }
    }
    for (Vertex u : fill_neighbors_[v]) {
        if (!IsRemoved(u)) {
            N_alpha.add(u);
        }
    }
    return N_alpha;
}

AdjacencyLists* EliminationAlgorithm::TriangNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(G_->order());
    if (fill_count_ == 0) {
        for (Vertex v : *G_) {
            a_lists->operator[](v).reserve(G_->N(v).size());
            for (Vertex u : G_->N(v)) {
                a_lists->operator[](v).add(u);
            }
        }
    } else {
        for (Vertex v : *G_) {
            a_lists->operator[](v).reserve(G_->N(v).size() +
                                           fill_neighbors_[v].size());
            for (Vertex u : G_->N(v)) {
                a_lists->operator[](v).add(u);
            }
            for (Vertex u : fill_neighbors_[v]) {
                a_lists->operator[](v).add(u);
            }
        }
    }
    return a_lists;
}

void EliminationAlgorithm::PrettyPrint() const {
    std::cout << "elimination order:\t";
    for (Vertex v : alpha_) {
        std::cout << G_->name(v) << " ";
    }
    std::cout << std::endl;
    std::cout << "tie distribution:\t";
    std::copy(tie_count_.begin(), tie_count_.end(),
              std::ostream_iterator< int >(std::cout, " "));
    std::cout << std::endl;
    return;
}

}  // namespace chordalg
