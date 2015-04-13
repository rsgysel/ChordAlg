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
#include <sstream>
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
    Graph(const Graph&, Vertices);
    explicit Graph(size_t);
    virtual ~Graph();

    bool IsIsomorphic(Graph&) const;
    virtual void PrettyPrint() const;
    void PrettyPrint(const LexTrie& T) const;
    void PrettyPrint(const VertexVector& U) const;
    void PrettyPrint(const Vertices& U) const;

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

    virtual Vertex vertex(char id) const;
    virtual Vertex vertex(std::string id) const;
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
    std::map<VertexName, Vertex> vertex_ids_;
    std::vector< std::vector< bool > > is_edge_;

    size_t order_;  // #vertices
    size_t size_;  // #edges

    // Induced Subgraph Initialization
    VertexNames* InducedNames(const Graph&, Vertices);
    AdjacencyLists* InducedVertices(const Graph&, Vertices);
    VertexNames* DefaultNames(size_t);

    template< class InputIterator >
    bool HasClique(InputIterator begin, InputIterator end) const;
};  // Graph

class Supergraph : public Graph {
 public:
    Supergraph() = delete;
    Supergraph(const Supergraph&) = delete;
    void operator=(const Supergraph&) = delete;

    Supergraph(const Graph*, AdjacencyLists*);

    Vertex vertex(char id) const {
        return G_->vertex(id);
    }
    Vertex vertex(std::string id) const {
        return G_->vertex(id);
    }
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
