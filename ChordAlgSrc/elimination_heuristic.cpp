#include "ChordAlgSrc/elimination_heuristic.h"

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

//////////////////
// c'tors & d'tors

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

EliminationHeuristic::EliminationHeuristic(
    const Graph* G,
    const EliminationParameters* parameters) :
    G_(G),
    parameters_(parameters),
    eo_(G_),
    fill_edges_(nullptr),
    remaining_vertices_(),
    ties_(),
    tie_count_(),
    B_(nullptr),
    unseparated_pairs_() {
    srand(time(nullptr));
    Init();
    return;
}

EliminationHeuristic::~EliminationHeuristic() {
    delete B_;
    return;
}

////////////////////////
// EliminationParameters

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

bool EliminationParameters::LBSeparation() const {
    return mode_ == EliminationMode::LBSEPARATION;
}

bool EliminationParameters::Mixed() const {
    return mode_ == EliminationMode::MIXED;
}

///////////////////////
// EliminationHeuristic

void EliminationHeuristic::Run() {
    delete fill_edges_;
    fill_edges_ = new FillEdges(G_);
    for (Vertex v : *G_) {
        remaining_vertices_.insert(v);
    }
    tie_count_.clear();
    if (parameters_->LBSeparation()) {
        // Monochromatic pair costs
        for (Vertex v : *G_) {
            for (Vertex u : *G_) {
                if (u != v) {
                Weight wt = G_->FillCount(u, v);
                    if (wt > 0) {
                        VertexPair uv =
                            VertexPair(std::min(u, v), std::max(u, v));
                        unseparated_pairs_[uv] = wt;
                    }
                }
            }
        }
    }
    Elimination();
    return;
}

std::string EliminationHeuristic::str() const {
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

const std::vector< size_t >& EliminationHeuristic::tie_count() const {
    return tie_count_;
}

const FillEdges& EliminationHeuristic::fill_edges() const {
    return *fill_edges_;
}

FillEdges* EliminationHeuristic::TakeFillEdges() {
    FillEdges* temp = nullptr;
    std::swap(temp, fill_edges_);
    return temp;
}

const EliminationParameters& EliminationHeuristic::parameters() const {
    return *parameters_;
}

void EliminationHeuristic::Init() {
    int n = G_->order();
    tie_count_.resize(n);
    if (!parameters_->Classic()) {
        B_ = new SeparatorBlocks(G_);
    }
    return;
}

void EliminationHeuristic::Elimination() {
    for (size_t i = 0; i < G_->order(); ++i) {
        VertexWeight vc = ArgMin();
        Vertex v = vc.first;
        eo_.Emplace(v, i);
        tie_count_[i] = ties_.size();
        ties_.clear();
        remaining_vertices_.erase(v);
        Eliminate(v);
    }
    return;
}

void EliminationHeuristic::Eliminate(Vertex v) {
    if (parameters_->Classic()) {
        Vertices Nv = MonotoneNbhd(v);
        fill_edges_->Saturate(Nv);
        return;
    } else {
        B_->SeparateClosedNbhd(v, fill_edges_);
        for (Block B : *B_) {
            for (VertexPair uv : VertexPairs(B.NC())) {
                fill_edges_->AddEdge(uv);
                if (parameters_->LBSeparation()) {
                    unseparated_pairs_.erase(uv);
                }
            }
        }
        return;
    }
}

VertexWeight EliminationHeuristic::ArgMin() {
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

VertexWeight EliminationHeuristic::TieBreak() {
    int i = rand() % ties_.size();
    return ties_[i];
}

Vertices EliminationHeuristic::MonotoneNbhd(Vertex v) {
    Vertices N_alpha;
    for (Vertex u : G_->N(v)) {
        if (!IsRemoved(u)) {
            N_alpha.push_back(u);
        }
    }
    for (Vertex u : (*fill_edges_)[v]) {
        if (!IsRemoved(u)) {
            N_alpha.push_back(u);
        }
    }
    return N_alpha;
}

bool EliminationHeuristic::IsRemoved(Vertex v) {
    return remaining_vertices_.find(v) == remaining_vertices_.end();
}

std::pair< Weight, Weight > EliminationHeuristic::WeightOf(Vertex v) {
    if (parameters_->LBElimination() || parameters_->LBSeparation()) {
        Weight deficiency_wt = 0, separated_wt = 0;
        B_->SeparateClosedNbhd(v, fill_edges_);
        // monochromatic fill pairs
        std::set< VertexPair > new_fill_edges;
        for (Block B : *B_) {
            for (VertexPair uw : VertexPairs(B.NC())) {
                if (!fill_edges_->IsEdge(uw)) {
                    new_fill_edges.insert(uw);
                }
            }
        }
        for (VertexPair uw : new_fill_edges) {
            deficiency_wt += G_->FillCount(uw);
        }
        if (parameters_->LBSeparation()) {
        // new monochromatic separation
            for (std::pair< VertexPair, Weight > p : unseparated_pairs_) {
                VertexPair uw = p.first;
                Vertex u = uw.first, w = uw.second;
                Weight fill_weight = p.second;
                if (B_->AreSeparated(u, w)) {
                    separated_wt += fill_weight;
                }
            }
        }
        return std::pair< Weight, Weight >(
            parameters_->ObjectiveFn(deficiency_wt, separated_wt),
            deficiency_wt);
    } else {
        Weight wt = 0;
        for (VertexPair uv : VertexPairs(MonotoneNbhd(v))) {
            if (!fill_edges_->IsEdge(uv)) {
                wt += G_->FillCount(uv);
            }
        }
        return std::pair< Weight, Weight >(parameters_->ObjectiveFn(wt), wt);
    }
}

}  // namespace chordalg
