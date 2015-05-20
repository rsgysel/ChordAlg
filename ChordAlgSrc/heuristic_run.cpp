#include "ChordAlgSrc/heuristic_run.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/elimination_heuristic.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

HeuristicRun::HeuristicRun(
    const Graph* G,
    const std::vector< EliminationParameters* >* elimination_parameters,
    bool atoms,
    size_t runs) :
    G_(G),
    elimination_parameters_(elimination_parameters),
    atoms_(atoms),
    runs_(runs),
    fill_edges_(new FillEdges(G)),
    fill_summary_(),
    run_summary_() {
    return;
}

HeuristicRun::~HeuristicRun() {
    delete fill_edges_;
}

///////////////
// HeuristicRun

const FillEdges& HeuristicRun::fill_edges() const {
    return *fill_edges_;
}

const std::string& HeuristicRun::fill_summary() const {
    return fill_summary_;
}

const std::string& HeuristicRun::run_summary() const {
    return run_summary_;
}

HeuristicRun* HeuristicRun::New(
    const CellIntersectionGraph* G,
    EliminationCriterion criterion,
    EliminationMode mode,
    bool atoms,
    size_t runs,
    float deficiency_wt,
    float separation_wt) {
    std::vector< EliminationCriterion > criteria(1, criterion);
    std::vector< EliminationMode > modes(1, mode);
    return HeuristicRun::New(
        G, criteria, modes, atoms, runs, deficiency_wt, separation_wt);
}

HeuristicRun* HeuristicRun::New(
    const CellIntersectionGraph* G,
    std::vector< EliminationCriterion > criteria,
    std::vector< EliminationMode > modes,
    bool atoms,
    size_t runs,
    float deficiency_wt,
    float separation_wt) {
    std::vector< EliminationParameters* > elimination_parameters;
    for (auto mode : modes) {
        for (auto criterion : criteria) {
            if (!Compatible(mode, criterion)) {
                continue;
            }
            auto P = new EliminationParameters(
                            criterion,
                            mode,
                            deficiency_wt,
                            separation_wt);
            elimination_parameters.push_back(P);
        }
    }
    HeuristicRun* R = new HeuristicRun(G, &elimination_parameters, atoms, runs);
    R->Run();
    for (auto P : elimination_parameters) {
        delete P;
    }
    // Return columns to remove
    std::set< Color > columns;
    for (Vertex v : *G) {
        for (Vertex u : R->fill_edges()[v]) {
            if (u < v) {
                for (Color c : G->CommonColors(u, v)) {
                    columns.insert(c);
                }
            }
        }
    }
    for (Color c : columns) {
        R->fill_summary_ += std::to_string(c) + " ";
    }
    return R;
}

void HeuristicRun::Run() {
    std::vector< const Graph* > graphs;
    Atoms* A = atoms_ ? Atoms::New(G_) : nullptr;
    if (atoms_) {
        for (auto a : *A) {
            graphs.push_back(a);
        }
    } else {
        graphs.push_back(G_);
    }
    size_t clique_atoms = 0, atom_id = 0, fill_count = 0;
    for (const Graph* G : graphs) {
        ++atom_id;
        if (!G->IsClique()) {
            double best_fill_weight = DBL_MAX;
            FillEdges* best_fill_edges = nullptr;
            for (auto parameters : *elimination_parameters_) {
                EliminationHeuristic algorithm(G, parameters);
                for (size_t i = 0; i < runs_; ++i) {
                    algorithm.Run();
                    const FillEdges& fill_edges = algorithm.fill_edges();
                    if (!best_fill_edges ||
                        fill_edges.fill_weight() < best_fill_weight) {
                        delete best_fill_edges;
                        best_fill_weight = fill_edges.fill_weight();
                        best_fill_edges = algorithm.TakeFillEdges();
                    }
                }
            }
            fill_count += best_fill_edges->fill_count();
            for (Vertex v : *G) {
                for (Vertex u : (*best_fill_edges)[v]) {
                    fill_edges_->AddEdge(G->ParentGraph(VertexPair(u, v)));
                }
            }
        } else {
            ++clique_atoms;
        }
    }
    run_summary_ = "characters removed: "
        + std::to_string(fill_edges_->fill_weight()) + '\n'
        + "fill edges added: " + std::to_string(fill_count) + '\n'
        + "vertices: " + std::to_string(G_->order()) + '\n'
        + "edges: " + std::to_string(G_->size()) + '\n';
    if (atoms_) {
        run_summary_ += "atom-subgraphs: " + std::to_string(A->size()) + '\n'
               + "clique atom-subgraphs: "
               + std::to_string(clique_atoms) + '\n';
        delete A;
    }
    return;
}

bool HeuristicRun::Compatible(EliminationMode mode, EliminationCriterion criterion) {
    if (mode == EliminationMode::CLASSIC) {
        if (criterion == EliminationCriterion::DEFICIENCY) {
            return true;
        }
    } else if (mode == EliminationMode::LBELIMINATION) {
        if (criterion == EliminationCriterion::RATIO ||
            criterion == EliminationCriterion::WSUM) {
            return true;
        }
    } else if (mode == EliminationMode::MIXED) {
        if (criterion == EliminationCriterion::DEFICIENCY) {
            return true;
        }
    }
    return false;
}

}  // namespace chordalg
