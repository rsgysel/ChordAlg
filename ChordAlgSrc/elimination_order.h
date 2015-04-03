/*
 *  elimination_ordering.h - data structure representing an elimination
 *  ordering of a graph
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

/*
 *  This data structure provides functionality used by triangulation
 *  algorithms based on classical elimination or LB-elimination.
 */

#ifndef INCLUDE_ELIMINATION_ORDER_H_
#define INCLUDE_ELIMINATION_ORDER_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include "atom_subgraphs.hpp"
#include "chordalg_types.h"
#include "file_reader.h"
#include "graph.h"
#include "vertex_utilities.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

struct EliminationCriterion {
    virtual Weight Calculate(Weight deficiency) = 0;
    virtual Weight Calculate(Weight deficiency, Weight separated) = 0;
};  // struct EliminationCriterion

class EliminationOrder {
 public:
    explicit EliminationOrder(Graph&);
    ~EliminationOrder();
    void Init();

    int ComputeFill();
    void Emplace(Vertex v, int i);
    void Swap(int i, int j);
    AdjacencyLists* TriangNbhds() const;

    void PrettyPrint() const;
    bool ZeroFill() const;

    bool Before(Vertex u, Vertex v) const {
        return alpha_inverse_[u] < alpha_inverse_[v];
    }
    int PositionOf(Vertex v) const {
        return alpha_inverse_[v];
    }
    Vertex VertexAt(int i) const {
        return alpha_[i];
    }

    // neighbors of v ``left" (before) v. Unsorted.
    Vertices LNbhd(Vertex v) const;
    // neighbors of v ``right" (after) v. Unsorted.
    Vertices RNbhd(Vertex v) const;

    int fill_count() const {
        return fill_count_;
    }
    int size() const {
        return alpha_.size();
    }
    const Graph& G() const {
        return G_;
    }

    // Mutators
    void SetOrder(VertexVector pi);
    void SetPosition(Vertex v, int i) {
        alpha_inverse_[v] = i;
    }
    void SetVertex(int i, Vertex v) {
        alpha_[i] = v;
    }

 private:
    Graph& G_;
    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< int > alpha_inverse_;

    int fill_count_;  // # of fill edges added (monochromatic or not)

    std::vector < VertexSet > triangulation_nbhds_;

    static int kUnfilled() {
        return -1;
    }
};  // class EliminationOrder

class EliminationAlgorithm {
 public:
    explicit EliminationAlgorithm(Graph& G);
    virtual ~EliminationAlgorithm();

    void PrettyPrint() const;

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

    Graph& G_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector < int > alpha_inverse_;

    Weight fill_cost_;
    int fill_count_;  // # of fill edges added (monochromatic or not)
    std::vector < VertexSet > fill_neighbors_;
    VertexSet remaining_vertices_;

    std::vector < VertexCost > ties_;
    std::vector < int > tie_count_;
};  // EliminationAlgorithm

template< class GraphType, class FileReaderType, class EliminationType,
          class CriterionType >
void RunAtomHeuristic(std::string filename,
                      CriterionType* criterion = new CriterionType(),
                      int runs = 1) {
    FileReaderType* graph_reader = NewFileReader < FileReaderType >(filename);
    GraphType G(graph_reader);

    Weight total_weight = 0;
    Atoms< GraphType > A(G);
    int clique_atoms = 0, atom_id = 0, total_count = 0;
    for (GraphType* a : A) {
        ++atom_id;
        if (!a->IsClique()) {
            EliminationType* best_eo = new EliminationType(*a, criterion);

            for (int i = 1; i < runs; ++i) {
                EliminationType* eo = new EliminationType(*a, criterion);
                if (eo->fill_cost() < best_eo->fill_cost()) {
                    delete best_eo;
                    best_eo = eo;
                }
            }

            total_weight += best_eo->fill_cost();
            total_count += best_eo->fill_count();
            std::cout << "atom " << atom_id << std::endl;
            best_eo->PrettyPrint();
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

    EliminationType eo(G, criterion);
    eo.PrettyPrint();

    std::cout << "fill weight: "    << eo.fill_cost()   << std::endl;
    std::cout << "fill count: "     << eo.fill_count()  << std::endl;
    std::cout << "vertices: "       << G.order()        << std::endl;
    std::cout << "edges: "          << G.size()         << std::endl;

    return;
}

}  // namespace chordalg

#endif  // INCLUDE_ELIMINATION_ORDER_H_