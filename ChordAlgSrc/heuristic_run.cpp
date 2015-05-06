#include "ChordAlgSrc/heuristic_run.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"

namespace chordalg {

bool Compatible(EliminationMode mode, EliminationCriterion criterion) {
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

std::string SetupAndRunHeuristic(
    std::string filename,
    std::vector< EliminationCriterion > criteria,
    std::vector< EliminationMode > modes,
    bool atoms,
    size_t runs,
    float deficiency_wt,
    float separation_wt) {
    CellIntersectionGraph* G = CellIntersectionGraph::New(filename);
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
    HeuristicRun R(G, &elimination_parameters, atoms, runs);
    std::cout << R.Run() << std::endl;
    for (auto P : elimination_parameters) {
        delete P;
    }
    // Return columns to remove
    std::set< Color > columns;
    for (VertexPair uv : R.fill_edges()) {
        for (Color c : G->CommonColors(uv)) {
            columns.insert(c);
        }
    }
    std::string columns_str;
    for (Color c : columns) {
        columns_str += std::to_string(c) + " ";
    }
    delete G;
    return columns_str;
}

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
    fill_edges_() {
    return;
}

///////////////
// HeuristicRun

std::vector< VertexPair > HeuristicRun::fill_edges() const {
    return fill_edges_;
}

std::string HeuristicRun::Run() {
    std::vector< const Graph* > graphs;
    Atoms* A = atoms_ ? Atoms::New(G_) : nullptr;
    if (atoms_) {
        for (auto a : *A) {
            graphs.push_back(a);
        }
    } else {
        graphs.push_back(G_);
    }
    fill_weight_ = 0;
    size_t clique_atoms = 0, atom_id = 0, fill_count = 0;
    for (const Graph* G : graphs) {
        ++atom_id;
        if (!G->IsClique()) {
            double best_fill_weight = DBL_MAX, best_fill_count = DBL_MAX;
            std::vector< VertexPair > best_fill_edges;
            for (auto parameters : *elimination_parameters_) {
                EliminationAlgorithm algorithm(G, parameters);
                for (size_t i = 0; i < runs_; ++i) {
                    algorithm.Run();
                    if (algorithm.fill_weight() < best_fill_weight) {
                        best_fill_weight = algorithm.fill_weight();
                        best_fill_count = algorithm.fill_count();
                        best_fill_edges = algorithm.fill_edges();
                    }
                }
            }
            fill_weight_ += best_fill_weight;
            fill_count += best_fill_count;
            for (VertexPair uv : best_fill_edges) {
                fill_edges_.push_back(G->ParentGraph(uv));
            }
        } else {
            ++clique_atoms;
        }
    }
    std::string log =
        "characters removed: " + std::to_string(fill_weight_) + '\n'
        + "fill edges added: " + std::to_string(fill_count) + '\n'
        + "vertices: " + std::to_string(G_->order()) + '\n'
        + "edges: " + std::to_string(G_->size()) + '\n';
    if (atoms_) {
        log += "atom-subgraphs: " + std::to_string(A->size()) + '\n'
               + "clique atom-subgraphs: "
               + std::to_string(clique_atoms) + '\n';
        delete A;
    }
    return log;
}

AdjacencyLists* HeuristicRun::TriangNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists(G_->neighbors());
    for (VertexPair uv : fill_edges_) {
        (*a_lists)[uv.first].push_back(uv.second);
        (*a_lists)[uv.second].push_back(uv.first);
    }
    return a_lists;
}

Weight HeuristicRun::fill_weight() const {
    return fill_weight_;
}

}  // namespace chordalg
