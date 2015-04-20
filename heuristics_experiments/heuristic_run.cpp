#include "heuristic_run.h"

bool Compatible(chordalg::EliminationMode mode, chordalg::EliminationCriterion criterion) {
    if (mode == chordalg::EliminationMode::CLASSIC) {
        if (criterion == chordalg::EliminationCriterion::DEFICIENCY) {
            return true;
        }
    } else if (mode == chordalg::EliminationMode::LBELIMINATION) {
        if (criterion == chordalg::EliminationCriterion::RATIO ||
            criterion == chordalg::EliminationCriterion::WSUM) {
            return true;
        }
    } else if (mode == chordalg::EliminationMode::MIXED) {
        if (criterion == chordalg::EliminationCriterion::DEFICIENCY) {
            return true;
        }
    }
    return false;
}

void SetupAndRunHeuristic(std::string filename,
                          std::vector< chordalg::EliminationCriterion > criteria,
                          std::vector< chordalg::EliminationMode > modes,
                          bool atoms,
                          size_t runs,
                          float deficiency_wt,
                          float separation_wt) {
    chordalg::MatrixCellIntGraphFR* F =
        chordalg::NewFileReader< chordalg::MatrixCellIntGraphFR >(filename);
    chordalg::ColoredIntersectionGraph G(F);
    std::vector< chordalg::EliminationParameters* > elimination_parameters;
    for (auto mode : modes) {
        for (auto criterion : criteria) {
            if (!Compatible(mode, criterion)) {
                continue;
            }
            auto P = new chordalg::EliminationParameters(
                            criterion,
                            mode,
                            deficiency_wt,
                            separation_wt);
            elimination_parameters.push_back(P);
        }
    }    
    HeuristicRun H(&G, &elimination_parameters, atoms, runs);
    std::cout << H.Run() << std::endl;
    for (auto P : elimination_parameters) {
        delete P;
    }
    delete F;
    return;
}


HeuristicRun::HeuristicRun(
    const chordalg::Graph* G,
    const std::vector< chordalg::EliminationParameters* >* elimination_parameters,
    bool atoms,
    size_t runs) :
    G_(G),
    elimination_parameters_(elimination_parameters),
    atoms_(atoms),
    runs_(runs),
    fill_edges_() {
    return;
}

const std::vector< chordalg::VertexPair >& HeuristicRun::fill_edges() const {
    return fill_edges_;
}

std::string HeuristicRun::Run() {
    std::vector< const chordalg::Graph* > graphs;
    chordalg::Atoms* A = atoms_ ? new chordalg::Atoms(G_) : nullptr;
    if (atoms_) {
        A->ComputeAtoms();
        for (auto a : *A) {
            graphs.push_back(a);
        }
    } else {
        graphs.push_back(G_);
    }
    chordalg::Weight total_fill_weight = 0;
    size_t clique_atoms = 0, atom_id = 0, total_fill_count = 0;
    for (const chordalg::Graph* G : graphs) {
        ++atom_id;
        if (!G->IsClique()) {
            double best_fill_weight = DBL_MAX, best_fill_count = DBL_MAX;
            std::vector< chordalg::VertexPair > best_fill_edges;
            for(auto parameters : *elimination_parameters_) {
                chordalg::EliminationAlgorithm algorithm(G, parameters);
                for(size_t i = 0; i < runs_; ++i) {
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
            for (chordalg::VertexPair uv : best_fill_edges) {
                if(G->FillCount(uv) > 0) {
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

