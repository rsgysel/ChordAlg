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
    std::vector < int > alpha_inverse_;

    Weight fill_cost_;
    int fill_count_;  // # of fill edges added (monochromatic or not)
    std::vector < VertexSet > fill_neighbors_;
    VertexSet remaining_vertices_;

    std::vector < VertexCost > ties_;
    std::vector < int > tie_count_;
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

template< class GraphType, class FileReaderType, class EliminationType,
          class CriterionType >
void RunAtomHeuristic(std::string filename,
                      CriterionType* criterion = new CriterionType(),
                      int runs = 1) {
    FileReaderType* graph_reader = NewFileReader < FileReaderType >(filename);
    GraphType G(graph_reader);
    delete graph_reader;

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

    return;
}

template< class GraphType, class FileReaderType, class EliminationType,
          class CriterionType >
void RunHeuristic(std::string filename,
                  CriterionType* criterion = new CriterionType()) {
    FileReaderType* graph_reader = NewFileReader < FileReaderType >(filename);
    GraphType G(graph_reader);
    delete graph_reader;

    EliminationType eo(&G, criterion);
    std::cout << eo.str() << std::endl;

    std::cout << "fill weight: "    << eo.fill_cost()   << std::endl;
    std::cout << "fill count: "     << eo.fill_count()  << std::endl;
    std::cout << "vertices: "       << G.order()        << std::endl;
    std::cout << "edges: "          << G.size()         << std::endl;

    return;
}

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ALGORITHM_H_
