#include "elimination_algorithm.h"

#include <algorithm>
#include <utility>

namespace chordalg {

////////////////////////
// EliminationParameters

EliminationParameters::EliminationParameters(EliminationCriterion criterion,
                                             bool atoms,
                                             size_t runs,
                                             float deficiency_wt,
                                             float separation_wt) :
    criterion_(criterion),
    atoms_(atoms),
    runs_(runs),
    deficiency_wt_(deficiency_wt),
    separation_wt_(separation_wt) {
    return;
}

Weight EliminationParameters::ObjectiveFn(Weight deficiency,
                                          Weight separated) const {
    if(criterion_ == EliminationCriterion::RATIO) {
        return deficiency / (1 + separated);
    } else if (criterion_ == EliminationCriterion::WSUM) {
        return deficiency_wt_*deficiency - separation_wt_*separated;
    } else if (criterion_ == EliminationCriterion::DEFICIENCY) {
        return deficiency;
    } else {
        std::cerr << "Error in EliminationParameters::ObjectiveFn: bad criterion" << std::endl;
        exit(EXIT_FAILURE);
    }
}

//////////////////////////////////
// Constructors, Destructors, Init

EliminationAlgorithm::EliminationAlgorithm(const Graph* G,
                                           const EliminationParameters* parameters) :
    G_(G),
    parameters_(parameters),
    alpha_(G_->order()),
    alpha_inverse_(),
    fill_weight_(0),
    fill_count_(0),
    fill_neighbors_(),
    fill_edges_(),
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
                                       const EliminationParameters* parameters) :
    EliminationAlgorithm(G, parameters) {
std::cout << __PRETTY_FUNCTION__ << '\n';
    EliminationAlgorithm::Init();
    return;
}

ClassicElimination::~ClassicElimination() {
    return;
}

LBElimination::LBElimination(const Graph* G,
                             const EliminationParameters* parameters) :
    EliminationAlgorithm(G, parameters),
    B_(G) {
std::cout << __PRETTY_FUNCTION__ << '\n';
    LBElimination::Init();
    return;
}

LBElimination::~LBElimination() {
    return;
}

void LBElimination::Init() {
    // Monochromatic pair costs
    for (Vertex v : *G_) {
        for (Vertex u : *G_) {
            Weight wt = G_->FillCount(u,v);
            if (u != v && wt > 0) {
                VertexPair uv = VertexPair(std::min(u, v), std::max(u, v));
                unseparated_pairs_[uv] = wt;
            }
        }
    }
    EliminationAlgorithm::Init();
    return;
}

MixedElimination::MixedElimination(const Graph* G,
                                   const EliminationParameters* parameters) :
    LBElimination(G, parameters),
    B_(G) {
std::cout << __PRETTY_FUNCTION__ << '\n';
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
        VertexWeight vc = ArgMin();
        fill_weight_ += vc.second;
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
        fill_edges_.push_back(p);
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

VertexWeight EliminationAlgorithm::ArgMin() {
    Weight min = MAX_WEIGHT;
    for (Vertex v : remaining_vertices_) {
        std::pair< Weight, Weight > wc = WeightOf(v);
        Weight wt = wc.first;
        Weight c = wc.second;

        if (wt < min) {
            min = wt;
            ties_.clear();
            ties_.push_back(VertexWeight(v, c));
        } else if (wt == min) {
            ties_.push_back(VertexWeight(v, c));
        }
    }
    return TieBreak();
}

VertexWeight EliminationAlgorithm::TieBreak() {
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
            (*a_lists)[v].reserve(G_->N(v).size());
            for (Vertex u : G_->N(v)) {
                (*a_lists)[v].add(u);
            }
        }
    } else {
        for (Vertex v : *G_) {
            (*a_lists)[v].reserve(G_->N(v).size() +
                                           fill_neighbors_[v].size());
            for (Vertex u : G_->N(v)) {
                (*a_lists)[v].add(u);
            }
            for (Vertex u : fill_neighbors_[v]) {
                (*a_lists)[v].add(u);
            }
        }
    }
    return a_lists;
}

std::string EliminationAlgorithm::str() const {
    std::string Estr = "elimination order:\t";
    for (Vertex v : alpha_) {
        Estr += G_->name(v) + " ";
    }
    Estr.erase(Estr.end() - 1, Estr.end());
    Estr += "\ntie distribution:\t";
    std::ostringstream oss;
    std::copy(tie_count_.begin(), tie_count_.end() - 1,
              std::ostream_iterator< int >(oss, " "));
    oss << tie_count_.back();
    Estr += oss.str() + '\n';
    return Estr;
}

/////////////////////
// ClassicElimination

void ClassicElimination::Eliminate(Vertex v) {
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        AddEdge(p);
    }
    return;
}

std::pair< Weight, Weight > ClassicElimination::WeightOf(Vertex v) {
    Weight c = 0;
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        if (!IsEdge(p)) {
            c += G_->FillCount(p);
        }
    }
    return std::pair< Weight, Weight >(parameters_->ObjectiveFn(c), c);
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

std::pair< Weight, Weight > LBElimination::WeightOf(Vertex v) {
    Weight deficiency_wt = 0, separated_wt = 0;
    Vertices S = MonotoneNbhd(v);
    S.add(v);
    B_.Separate(S , fill_neighbors_);
    // monochromatic fill pairs
    std::set< VertexPair > seen_fill_pairs;
    for (Block B : B_) {
        for (VertexPair uw : VertexPairs(B.NC())) {
            Weight fill_weight = G_->FillCount(uw);
            if (!IsEdge(uw) && fill_weight > 0 &&
                seen_fill_pairs.find(uw) == seen_fill_pairs.end()) {
                deficiency_wt += fill_weight;
                seen_fill_pairs.insert(uw);
            }
        }
    }
    // new monochromatic separation
    for (std::pair< VertexPair, Weight > p : unseparated_pairs_) {
        VertexPair uw = p.first;
        Vertex u = uw.first, w = uw.second;
        Weight fill_weight = p.second;
        if (B_.AreSeparated(u, w)) {
            separated_wt += fill_weight;
        }
    }
    return std::pair< Weight, Weight>(
        parameters_->ObjectiveFn(deficiency_wt, separated_wt), deficiency_wt);
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

std::pair< Weight, Weight > MixedElimination::WeightOf(Vertex v) {
    Weight wt = 0;
    for (VertexPair p : VertexPairs(MonotoneNbhd(v))) {
        if (!IsEdge(p)) {
            wt += parameters_->ObjectiveFn(G_->FillCount(p));
        }
    }
    // Don't need separation statistics
    return std::pair<Weight, Weight>(wt, wt);
}

}  // namespace chordalg
