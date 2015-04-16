/*
 *  graph.h - a graph data structure
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

#ifndef INCLUDE_GRAPH_H_
#define INCLUDE_GRAPH_H_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "file_reader.h"
#include "lex_trie.h"
#include "vertices.h"

namespace chordalg {

class Graph {
 public:
    Graph() = delete;
    Graph(const Graph&) = delete;
    void operator=(const Graph&) = delete;

    explicit Graph(Graph&);
    explicit Graph(FileReader*);
    explicit Graph(AdjacencyLists*);
    Graph(AdjacencyLists*, VertexNames);
    explicit Graph(size_t);
    virtual ~Graph();

    bool IsIsomorphic(Graph&) const;
    virtual std::string str() const;
    std::string str(const LexTrie& T) const;
    std::string str(const VertexVector& U) const;
    std::string str(const Vertices& U) const;

    Vertices V() const {
        Vertices V(order_);
        for(size_t i = 0; i < V.size(); ++i ) {
            V[i] = i;
        }
        return V;
    }
    GraphVertexIterator begin() const {
        return GraphVertices(this, order_).begin();
    }
    GraphVertexIterator end() const {
        return GraphVertices(this, order_).end();
    }

    virtual VertexName name(Vertex v) const {
        return vertex_names_->operator[](v);
    }
    size_t order() const {
        return order_;
    }
    size_t size() const {
        return size_;
    }

    virtual bool HasEdge(Vertex u, Vertex v) const {
        return is_edge_[u][v];
    }
    virtual bool HasEdge(VertexPair p) const {
        return HasEdge(p.first, p.second);
    }
    virtual Cost FillCost(Vertex u, Vertex v) const {
        return HasEdge(u, v) ? 0 : 1;
    }
    virtual Cost FillCost(VertexPair p) const {
        return FillCost(p.first, p.second);
    }
    template< class Container >
    bool HasClique(Container set) const {
        return HasClique(set.begin(), set.end());
    }
    bool IsClique() const {
        return 2 * size_ == order_ * (order_ - 1);
    }
    const Vertices& N(Vertex v) const {
        return neighborhoods_->operator[](v);
    }

 protected:
    void Init();

    const AdjacencyLists* const neighborhoods_;
    const VertexNames* const vertex_names_;
    std::vector< std::vector< bool > > is_edge_;

    size_t order_;  // #vertices
    size_t size_;  // #edges

    // Induced Subgraph Initialization
    AdjacencyLists* InducedVertices(const Graph&, Vertices);
    VertexNames* DefaultNames(size_t);

    template< class InputIterator >
    bool HasClique(InputIterator begin, InputIterator end) const;
};  // Graph

class InducedSubgraph : public Graph {
 public:
    InducedSubgraph() = delete;
    InducedSubgraph(const InducedSubgraph&) = delete;
    void operator=(const InducedSubgraph&) = delete;

    InducedSubgraph(const Graph*, Vertices);

    VertexName name(Vertex v) const {
        return G_->name(U_[v]);
    }
    Cost Fillcost(Vertex u, Vertex v) const {
        return G_->FillCost(U_[u], U_[v]);
    }

 protected:
    const Graph* const G_;
    const Vertices U_;
};  // InducedSubgraph

class Supergraph : public Graph {
 public:
    Supergraph() = delete;
    Supergraph(const Supergraph&) = delete;
    void operator=(const Supergraph&) = delete;

    Supergraph(const Graph*, AdjacencyLists*);
    VertexName name(Vertex v) const {
        return G_->name(v);
    }

 protected:
    const Graph* const G_;     // G = (V,E) is a subgraph of H = (V,E U F)
};  // Supergraph

////////////// Generics

template< class InputIterator >
bool Graph::HasClique(InputIterator begin, InputIterator end) const {
    for (InputIterator vertex_itr = begin; vertex_itr != end; ++vertex_itr) {
        InputIterator neighbor_itr = vertex_itr;
        for (++neighbor_itr; neighbor_itr != end; ++neighbor_itr) {
            if (!HasEdge(*vertex_itr, *neighbor_itr)) {
                return false;
            }
        }
    }

    return true;
}

}  // namespace chordalg

#endif  // INCLUDE_GRAPH_H_
