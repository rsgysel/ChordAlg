#include "elimination_algorithm.h"

#include <algorithm>
#include <utility>

namespace chordalg {

//////////////////////////////////
// Constructors, Destructors, Init

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

LBElimination::LBElimination(const Graph* G, const LBCriterion* f) :
    EliminationAlgorithm(G),
    f_(f),
    B_(G) {
    LBElimination::Init();
    return;
}

LBElimination::~LBElimination() {
    delete f_;
    return;
}

void LBElimination::Init() {
    // Monochromatic pair costs
    for (Vertex v : *G_) {
        for (Vertex u : *G_) {
            Cost c = G_->FillCost(u,v);
            if (u != v && c > 0) {
                VertexPair uv = VertexPair(std::min(u, v), std::max(u, v));
                unseparated_pairs_[uv] = c;
            }
        }
    }
    EliminationAlgorithm::Init();
    return;
}

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

///////////////////////
// EliminationAlgorithm

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

std::string EliminationAlgorithm::str() const {
    std::ostringstream oss;
    oss << "elimination order:\t";
    for (Vertex v : alpha_) {
        oss << G_->name(v) << " ";
    }
    oss << std::endl;
    oss << "tie distribution:\t";
    std::copy(tie_count_.begin(), tie_count_.end(),
              std::ostream_iterator< int >(oss, " "));
    oss << std::endl;
    return oss.str();
}

/////////////////////
// ClassicElimination

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


////////////////
// LBElimination

void LBElimination::Eliminate(Vertex v) {
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

std::pair< Weight, Cost > LBElimination::WeightOf(Vertex v) {
    Weight deficiency_wt = 0, separated_wt = 0;
    Vertices S = MonotoneNbhd(v);
    S.add(v);
    B_.Separate(S , fill_neighbors_);
    // monochromatic fill pairs
    std::set< VertexPair > seen_fill_pairs;
    for (Block B : B_) {
        for (VertexPair uw : VertexPairs(B.NC())) {
            Cost fill_cost = G_->FillCost(uw);
            if (!IsEdge(uw) && fill_cost > 0 &&
               seen_fill_pairs.find(uw) == seen_fill_pairs.end()) {
                deficiency_wt += fill_cost;
                seen_fill_pairs.insert(uw);
            }
        }
    }
    // new monochromatic separation
    for (std::pair< VertexPair, Weight > p : unseparated_pairs_) {
        VertexPair uw = p.first;
        Vertex u = uw.first, w = uw.second;
        Weight fill_cost = p.second;
        if (B_.AreSeparated(u, w)) {
            separated_wt += fill_cost;
        }
    }
    return std::pair< Weight, Cost>(f_->Calculate(deficiency_wt, separated_wt),
                                    deficiency_wt);
}

std::pair< Weight, Cost > LBElimination::ObjectiveFunction(
    Weight deficiency_wt, Weight separated_wt) {
    return std::pair< Weight, Cost >(deficiency_wt / ( 1 + separated_wt ),
                                     deficiency_wt);
}

///////////////////
// MixedElimination

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
