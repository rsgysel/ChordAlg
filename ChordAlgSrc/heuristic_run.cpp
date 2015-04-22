#include "heuristic_run.h"

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
    MatrixCellIntGraphFR* F =
        NewFileReader< MatrixCellIntGraphFR >(filename);
    ColoredIntersectionGraph G(F);
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
    HeuristicRun R(&G, &elimination_parameters, atoms, runs);
    std::cout << R.Run() << std::endl;
    for (auto P : elimination_parameters) {
        delete P;
    }
    delete F;
    // Return columns to remove
    std::set< Color > columns;
    for (VertexPair uv : R.fill_edges()) {
        for (Color c : G.CommonColors(uv)) {
            columns.insert(c);
        }
    }
    std::string columns_str;
    for (Color c : columns) {
        columns_str += std::to_string(c) + " ";
    }
    return columns_str;
}


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

std::vector< VertexPair > HeuristicRun::fill_edges() const {
    return fill_edges_;
}

std::string HeuristicRun::Run() {
    std::vector< const Graph* > graphs;
    Atoms* A = atoms_ ? new Atoms(G_) : nullptr;
    if (atoms_) {
        A->ComputeAtoms();
        for (auto a : *A) {
            graphs.push_back(a);
        }
    } else {
        graphs.push_back(G_);
    }
    Weight total_fill_weight = 0;
    size_t clique_atoms = 0, atom_id = 0, total_fill_count = 0;
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
            total_fill_weight += best_fill_weight;
            total_fill_count += best_fill_count;
            for (VertexPair uv : best_fill_edges) {
                if (G->FillCount(uv) > 0) {
                    fill_edges_.push_back(uv);
                }
            }
        } else {
            ++clique_atoms;
        }
    }
    std::string log = "fill weight: " + std::to_string(total_fill_weight) + '\n'
                      + "fill count: " + std::to_string(total_fill_count) + '\n'
                      + "vertices: " + std::to_string(G_->order()) + '\n'
                      + "edges : " + std::to_string(G_->size()) + '\n';
    if (atoms_) {
        log += "atoms: " + std::to_string(A->size()) + '\n'
               + "clique atoms: " + std::to_string(clique_atoms) + '\n';
        delete A;
    }
    return log;
}

AdjacencyLists* HeuristicRun::TriangNbhds() const {
    AdjacencyLists* a_lists = new AdjacencyLists(G_->neighbors());
    for (VertexPair uv : fill_edges_) {
        (*a_lists)[uv.first].add(uv.second);
        (*a_lists)[uv.second].add(uv.first);
    }
    return a_lists;
}

}  // namespace chordalg
