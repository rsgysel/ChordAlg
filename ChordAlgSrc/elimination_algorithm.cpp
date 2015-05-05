#include "ChordAlgSrc/elimination_algorithm.h"

#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

////////////////////////
// EliminationParameters

EliminationParameters::EliminationParameters(
    EliminationCriterion criterion,
    EliminationMode mode,
    float deficiency_wt,
    float separation_wt) :
    criterion_(criterion),
    mode_(mode),
    deficiency_wt_(deficiency_wt),
    separation_wt_(separation_wt) {
    return;
}

Weight EliminationParameters::ObjectiveFn(
    Weight deficiency,
    Weight separated) const {
    if (criterion_ == EliminationCriterion::RATIO) {
        return deficiency / (1 + separated);
    } else if (criterion_ == EliminationCriterion::WSUM) {
        return deficiency_wt_*deficiency - separation_wt_*separated;
    } else if (criterion_ == EliminationCriterion::DEFICIENCY) {
        return deficiency;
    } else {
        std::cerr <<
            "Error in EliminationParameters::ObjectiveFn: bad criterion" <<
            std::endl;
        exit(EXIT_FAILURE);
    }
}

bool EliminationParameters::Classic() const {
    return mode_ == EliminationMode::CLASSIC;
}

bool EliminationParameters::LBElimination() const {
    return mode_ == EliminationMode::LBELIMINATION;
}

bool EliminationParameters::Mixed() const {
    return mode_ == EliminationMode::MIXED;
}

////////////////////////////
// Constructors, Destructors

EliminationAlgorithm::EliminationAlgorithm(
    const Graph* G,
    const EliminationParameters* parameters) :
    G_(G),
    parameters_(parameters),
    eo_(G_),
    fill_weight_(),
    fill_count_(),
    fill_neighbors_(),
    fill_edges_(),
    remaining_vertices_(),
    ties_(),
    tie_count_(),
    B_(nullptr),
    unseparated_pairs_() {
    srand(time(nullptr));
    Init();
    return;
}

EliminationAlgorithm::~EliminationAlgorithm() {
    delete B_;
    return;
}

////////////
// Init, Run

void EliminationAlgorithm::Init() {
    int n = G_->order();
    fill_neighbors_.resize(n);
    tie_count_.resize(n);
    if (!parameters_->Classic()) {
        B_ = new SeparatorBlocks(G_);
    }
    return;
}

void EliminationAlgorithm::Run() {
    fill_weight_ = fill_count_ = 0;
    fill_edges_.clear();
    for (Vertex v : *G_) {
        remaining_vertices_.insert(v);
        fill_neighbors_[v].clear();
    }
    tie_count_.clear();
    if (!parameters_->Classic()) {
        // Monochromatic pair costs
        for (Vertex v : *G_) {
            for (Vertex u : *G_) {
                Weight wt = G_->FillCount(u, v);
                if (u != v && wt > 0) {
                    VertexPair uv = VertexPair(std::min(u, v), std::max(u, v));
                    unseparated_pairs_[uv] = wt;
                }
            }
        }
    }
    Elimination();
    return;
}

//////////////
// Elimination

void EliminationAlgorithm::Elimination() {
    for (size_t i = 0; i < G_->order(); ++i) {
        VertexWeight vc = ArgMin();
        fill_weight_ += vc.second;
        Vertex v = vc.first;
        eo_.Emplace(v, i);
        tie_count_[i] = ties_.size();
        ties_.clear();
        remaining_vertices_.erase(v);
        Eliminate(v);
    }
    return;
}

void EliminationAlgorithm::Eliminate(Vertex v) {
    if (parameters_->Classic()) {
        for (VertexPair uv : VertexPairs(MonotoneNbhd(v))) {
            AddEdge(uv);
        }
        return;
    } else {
        Vertices S = MonotoneNbhd(v);
        S.push_back(v);
        B_->Separate(S, fill_neighbors_);
        for (Block B : *B_) {
            for (VertexPair uv : VertexPairs(B.NC())) {
                AddEdge(uv);
                unseparated_pairs_.erase(uv);
            }
        }
        return;
    }
}

//////////////////////////
// Edge and Fill Functions

void EliminationAlgorithm::AddEdge(VertexPair uv) {
    if (!IsEdge(uv)) {
        fill_neighbors_[uv.first].insert(uv.second);
        fill_neighbors_[uv.second].insert(uv.first);
        fill_edges_.push_back(uv);
        ++fill_count_;
    }
    return;
}

bool EliminationAlgorithm::IsEdge(VertexPair uv) {
    return G_->HasEdge(uv) || IsFillEdge(uv);
}

bool EliminationAlgorithm::IsFillEdge(VertexPair uv) {
    return fill_neighbors_[uv.first].find(uv.second) !=
           fill_neighbors_[uv.first].end();
}

bool EliminationAlgorithm::IsRemoved(Vertex v) {
    return remaining_vertices_.find(v) == remaining_vertices_.end();
}

void EliminationAlgorithm::Saturate(Vertices U) {
    for (VertexPair uv : VertexPairs(U)) {
        if (!IsEdge(uv)) {
            Vertex u = uv.first, v = uv.second;
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
            N_alpha.push_back(u);
        }
    }
    for (Vertex u : fill_neighbors_[v]) {
        if (!IsRemoved(u)) {
            N_alpha.push_back(u);
        }
    }
    return N_alpha;
}

AdjacencyLists* EliminationAlgorithm::TriangNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists(G_->neighbors());
    for (Vertex v : *G_) {
        for (Vertex u : fill_neighbors_[v]) {
            (*a_lists)[v].push_back(u);
        }
    }
    return a_lists;
}

std::string EliminationAlgorithm::str() const {
    std::string Estr = "elimination order:\t";
    Estr += eo_.str();
    Estr += "\ntie distribution:\t";
    std::ostringstream oss;
    std::copy(tie_count_.begin(), tie_count_.end() - 1,
              std::ostream_iterator< int >(oss, " "));
    oss << tie_count_.back();
    Estr += oss.str() + '\n';
    return Estr;
}

std::pair< Weight, Weight > EliminationAlgorithm::WeightOf(Vertex v) {
    if (parameters_->LBElimination()) {
        Weight deficiency_wt = 0, separated_wt = 0;
        Vertices S = MonotoneNbhd(v);
        S.push_back(v);
        B_->Separate(S , fill_neighbors_);
        // monochromatic fill pairs
        std::set< VertexPair > seen_fill_pairs;
        for (Block B : *B_) {
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
            if (B_->AreSeparated(u, w)) {
                separated_wt += fill_weight;
            }
        }
        return std::pair< Weight, Weight>(
            parameters_->ObjectiveFn(deficiency_wt, separated_wt),
            deficiency_wt);
    } else {
        Weight wt = 0;
        for (VertexPair uv : VertexPairs(MonotoneNbhd(v))) {
            if (!IsEdge(uv)) {
                wt += G_->FillCount(uv);
            }
        }
        return std::pair< Weight, Weight >(parameters_->ObjectiveFn(wt), wt);
    }
}

}  // namespace chordalg
