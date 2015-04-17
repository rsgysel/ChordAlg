/*
 *  elimination_algorithm.h - base class for vertex elimination algorithms
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_ELIMINATION_ALGORITHM_H_
#define INCLUDE_ELIMINATION_ALGORITHM_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "atoms.h"
#include "elimination_order.h"
#include "file_reader.h"
#include "graph.h"
#include "separator.h"
#include "vertices.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

struct EliminationCriterion {
    virtual Weight Calculate(Weight deficiency) const = 0;
    virtual Weight Calculate(Weight deficiency, Weight separated) const = 0;
};  // struct EliminationCriterion

class EliminationAlgorithm {
 public:
    EliminationAlgorithm() = delete;
    EliminationAlgorithm(const EliminationAlgorithm&) = delete;
    void operator=(const EliminationAlgorithm&) = delete;

    explicit EliminationAlgorithm(const Graph* G);
    virtual ~EliminationAlgorithm();

    std::string str() const;

    Weight fill_cost() const {
        return fill_cost_;
    }
    int fill_count() const {
        return fill_count_;
    }
    const std::vector< int >& tie_count() const {
        return tie_count_;
    }
    const std::vector< VertexSet >& fill_neighbors() const {
        return fill_neighbors_;
    }
    const std::vector< VertexPair >& fill_edges() const {
        return fill_edges_;
    }

    AdjacencyLists* TriangNbhds() const;

 protected:
    virtual void Init();

    // Main Methods
    void Elimination();
    VertexCost ArgMin();
    virtual VertexCost TieBreak();  // base method breaks ties uniformly
    Vertices MonotoneNbhd(Vertex);

    // Helpers
    void AddEdge(VertexPair);
    bool IsEdge(VertexPair);
    bool IsFillEdge(VertexPair);
    bool IsRemoved(Vertex);
    void Saturate(Vertices);

    virtual void Eliminate(Vertex) = 0;
    virtual std::pair< Weight, Cost > WeightOf(Vertex) = 0;

    const Graph* const G_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< int > alpha_inverse_;

    Weight fill_cost_;
    int fill_count_;  // # of fill edges added (monochromatic or not)
    std::vector< VertexSet > fill_neighbors_;
    std::vector< VertexPair > fill_edges_;
    VertexSet remaining_vertices_;

    std::vector< VertexCost > ties_;
    std::vector< int > tie_count_;
};  // EliminationAlgorithm

struct ClassicCriterion : public EliminationCriterion {
 public:
    virtual ~ClassicCriterion() {}
    virtual Weight Calculate(Weight) const {
        return 0;
    }

 private:
    Weight Calculate(Weight, Weight) const {
        return 0;
    }
};  // ClassicCriterion

struct DeficiencyCriterion : public ClassicCriterion {
 public:
    virtual ~DeficiencyCriterion() {}
    Weight Calculate(Weight deficiency) const {
        return deficiency;
    }
};  // DeficiencyCriterion

class ClassicElimination : public EliminationAlgorithm {
 public:
    explicit ClassicElimination(const Graph*, const ClassicCriterion*);
    virtual ~ClassicElimination();

 private:
    void Eliminate(Vertex);
    std::pair< Weight, Cost > WeightOf(Vertex);

    const ClassicCriterion* const f_;
};  // class ClassicEliminationi

struct LBCriterion : public EliminationCriterion {
 public:
    virtual ~LBCriterion() {}
    virtual Weight Calculate(Weight, Weight) const {
        return 0;
    }
 private:
    virtual Weight Calculate(Weight) const {
        return 0;
    }
};  // LBCriterion

struct RatioCriterion : public LBCriterion {
 public:
    virtual ~RatioCriterion() {}
    Weight Calculate(Weight deficiency, Weight separated) const {
        return deficiency / (1 + separated);
    }
};  // RatioCriterion

struct WSumCriterion : public LBCriterion {
 public:
    virtual ~WSumCriterion() {}
    WSumCriterion() : d_(1), s_(1) {}
    WSumCriterion(Weight d, Weight s) : d_(d), s_(s) {}

    Weight Calculate(Weight deficiency, Weight separated) const {
        return d_*deficiency - s_*separated;
    }
 private:
    Weight d_, s_;
};  // WSumCriterion

class LBElimination : public EliminationAlgorithm {
 public:
    LBElimination() = delete;
    LBElimination(const LBElimination&) = delete;
    void operator=(const LBElimination&) = delete;

    explicit LBElimination(const Graph*, const LBCriterion*);
    virtual ~LBElimination();

 protected:
    void Init();
    void Eliminate(Vertex);
    std::pair< Weight, Cost > WeightOf(Vertex);

    virtual std::pair< Weight, Cost > ObjectiveFunction(Weight, Weight);

    const LBCriterion* const f_;

    SeparatorBlocks B_;

    std::map< VertexPair, Weight > unseparated_pairs_;
};  // class LBElimination

class MixedElimination : public LBElimination {
 public:
    MixedElimination() = delete;
    MixedElimination(const MixedElimination&) = delete;
    void operator=(const MixedElimination&) = delete;

    explicit MixedElimination(const Graph*, const LBCriterion*);
    virtual ~MixedElimination();

 private:
    void Eliminate(Vertex);
    std::pair< Weight, Cost > WeightOf(Vertex);

    SeparatorBlocks B_;
};  // MixedElimination

template< class EliminationType >
class HeuristicRun {
 public:
    HeuristicRun() = delete;
    HeuristicRun(const HeuristicRun&) = delete;
    void operator=(const HeuristicRun&) = delete;

    HeuristicRun(const Graph* G, const EliminationCriterion* criterion) :
        G_(G), criterion_(criterion), atoms_(false), runs_(1) {}

    void atoms() {
        atoms_ = true;
    }

    const std::vector< VertexPair >& fill_edges() const {
        return fill_edges;
    }

    void runs(size_t n) {
        runs_ = n;
    }

    std::string Run() {
        std::vector< const Graph* > graphs;
        Atoms* A = atoms_ ? new Atoms(G_) : nullptr;
        if (atoms_) {
            A->ComputeAtoms();
            graphs.resize(A->size());
            for (auto a : *A) {
                graphs.push_back(a);
            }
        } else {
            graphs.push_back(G_);
        }
        Weight total_fill_weight = 0;
        size_t clique_atoms = 0, atom_id = 0, total_fill_count = 0;
        for (auto G : graphs) {
            ++atom_id;
            if (!G->IsClique()) {
                EliminationType* best_eo = new EliminationType(G, criterion_);
                for(size_t i = 1; i < runs_; ++i) {
                    EliminationType* eo = new EliminationType(G, criterion_);
                    if (eo->fill_cost() < best_eo->fill_cost()) {
                        delete best_eo;
                        best_eo = eo;
                    } else {
                        delete eo;
                    }
                }
                total_fill_weight += best_eo->fill_cost();
                total_fill_count += best_eo->fill_count();
                for (auto uv : best_eo->fill_edges()) {
                    if(G->FillCost(uv) > 0) {
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
        if (atoms_) {
            log += "atoms: " + std::to_string(A->size()) + '\n'
                   + "clique atoms: " + std::to_string(clique_atoms) + '\n';
        }
        delete A;
        return log;
    } 

 protected:
    const Graph* const G_;
    const EliminationCriterion* const criterion_;
    bool atoms_;
    size_t runs_;

    std::vector< VertexPair > fill_edges_;
};  // HeuristicRun

template< class GraphType, class FileReaderType, class EliminationType,
          class CriterionType >
std::vector< VertexPair > RunAtomHeuristic(std::string filename,
                          CriterionType* criterion = new CriterionType(),
                          int runs = 1) {
    FileReaderType* graph_reader = NewFileReader < FileReaderType >(filename);
    GraphType G(graph_reader);
    delete graph_reader;

    std::vector< VertexPair > fill_edges;
    Weight total_weight = 0;
    Atoms A(&G);
    A.ComputeAtoms();
    int clique_atoms = 0, atom_id = 0, total_count = 0;
    for (auto a : A) {
        ++atom_id;
        if (!a->IsClique()) {
            EliminationType* best_eo = new EliminationType(a, criterion);

            for (int i = 1; i < runs; ++i) {
                EliminationType* eo = new EliminationType(a, criterion);
                if (eo->fill_cost() < best_eo->fill_cost()) {
                    delete best_eo;
                    best_eo = eo;
                }
            }

            total_weight += best_eo->fill_cost();
            total_count += best_eo->fill_count();
            std::cout << "atom " << atom_id << std::endl;
            std::cout << best_eo->str() << std::endl;

            for(auto uv : best_eo->fill_edges()) {
                if(a->FillCost(uv) > 0) {
                    fill_edges.push_back(uv);
                }
            }
            delete best_eo;
        } else {
            ++clique_atoms;
        }
    }

    std::cout << "fill weight: "    << total_weight << std::endl;
    std::cout << "fill count: "     << total_count  << std::endl;
    std::cout << "vertices: "       << G.order()    << std::endl;
    std::cout << "edges: "          << G.size()     << std::endl;
    std::cout << "atoms: "          << A.size()     << std::endl;
    std::cout << "clique atoms: "   << clique_atoms << std::endl;

    return fill_edges;
}

template< class GraphType, class FileReaderType, class EliminationType,
          class CriterionType >
std::vector< VertexPair > RunHeuristic(std::string filename,
                          CriterionType* criterion = new CriterionType()) {
    FileReaderType* graph_reader = NewFileReader < FileReaderType >(filename);
    GraphType G(graph_reader);
    delete graph_reader;

    EliminationType eo(&G, criterion);
    std::cout << eo.str() << std::endl;
    std::vector< VertexPair > fill_edges;
    for(auto uv : eo.fill_edges()) {
        if(G.FillCost(uv) > 0) {
            fill_edges.push_back(uv);
        }
    }
    std::cout << "fill weight: "    << eo.fill_cost()   << std::endl;
    std::cout << "fill count: "     << eo.fill_count()  << std::endl;
    std::cout << "vertices: "       << G.order()        << std::endl;
    std::cout << "edges: "          << G.size()         << std::endl;

    return fill_edges;
}

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ALGORITHM_H_
