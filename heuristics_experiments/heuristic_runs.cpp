#include "heuristic_runs.h"

void SetupAndRunHeuristic(std::string filename,
                          std::set< std::string > elimination_types,
                          chordalg::EliminationCriterion criterion,
                          bool atoms,
                          size_t runs,
                          float deficiency_wt,
                          float separation_wt) {
    chordalg::MatrixCellIntGraphFR* F =
        chordalg::NewFileReader< chordalg::MatrixCellIntGraphFR >(filename);
    chordalg::ColoredIntersectionGraph G(F);
    chordalg::EliminationParameters P(criterion, atoms, runs, deficiency_wt, separation_wt);
    HeuristicRun H(&G, &elimination_types, &P);
    std::cout << H.Run() << std::endl;
    delete F;
    return;
}


HeuristicRun::HeuristicRun(const chordalg::Graph* G,
                           const std::set< std::string >* elimination_types,
                           const chordalg::EliminationParameters* parameters) :
                           G_(G),
                           elimination_types_(elimination_types),
                           parameters_(parameters),
                           fill_edges_() {
    return;
}

const std::vector< chordalg::VertexPair >& HeuristicRun::fill_edges() const {
    return fill_edges_;
}

chordalg::EliminationAlgorithm* HeuristicRun::SingleEliminationRun(
    const std::string elimination_type,
    const chordalg::Graph* G) const {
    if (elimination_type.compare("ClassicElimination")) {
        return new chordalg::ClassicElimination(G, parameters_);
    } else if (elimination_type.compare("LBElimination")) {
        return new chordalg::LBElimination(G, parameters_);
    } else if (elimination_type.compare("MixedElimination")) {
        return new chordalg::MixedElimination(G, parameters_);
    } else {
        std::cerr << "HeuristicRun::SingleEliminationRun: unknown type\n";
        exit(EXIT_FAILURE);
    }
}

std::string HeuristicRun::Run() {
    std::vector< const chordalg::Graph* > graphs;
    chordalg::Atoms* A = parameters_->atoms() ? new chordalg::Atoms(G_) : nullptr;
    if (parameters_->atoms()) {
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
            chordalg::EliminationAlgorithm* best_eo = nullptr;
            for(size_t i = 0; i < parameters_->runs(); ++i) {
                for(auto elimination_type : *elimination_types_) {
                    chordalg::EliminationAlgorithm* eo =
                        SingleEliminationRun(elimination_type, G);
                    if (!best_eo) {
                        best_eo = eo;
                    } else if (eo->fill_weight() < best_eo->fill_weight()) {
                        delete best_eo;
                        best_eo = eo;
                    } else {
                        delete eo;
                    }
                }
            }
            total_fill_weight += best_eo->fill_weight();
            total_fill_count += best_eo->fill_count();
            for (auto uv : best_eo->fill_edges()) {
                if(G->FillCount(uv) > 0) {
                    fill_edges_.push_back(uv);
                }
            }
            delete best_eo;
        } else {
            ++clique_atoms;
        }
    }
    std::string log = "fill weight: " + std::to_string(total_fill_weight) + '\n'
                      + "fill count: " + std::to_string(total_fill_count) + '\n'
                      + "vertices: " + std::to_string(G_->order()) + '\n'
                      + "edges : " + std::to_string(G_->size()) + '\n';
    if (parameters_->atoms()) {
        log += "atoms: " + std::to_string(A->size()) + '\n'
               + "clique atoms: " + std::to_string(clique_atoms) + '\n';
        delete A;
    }
    return log;
}

