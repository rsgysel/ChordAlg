/*
 *  graph.h - a graph data structure
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

    const AdjacencyLists& neighbors() const {
        return *neighborhoods_;
    }
    virtual VertexName name(Vertex v) const {
        return (*vertex_names_)[v];
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
    virtual Weight FillCount(Vertex u, Vertex v) const {
        return HasEdge(u, v) ? 0 : 1;
    }
    virtual Weight FillCount(VertexPair p) const {
        return FillCount(p.first, p.second);
    }
    template< class Container >
    bool HasClique(Container set) const {
        return HasClique(set.begin(), set.end());
    }
    bool IsClique() const {
        return 2 * size_ == order_ * (order_ - 1);
    }
    const Vertices& N(Vertex v) const {
        return (*neighborhoods_)[v];
    }

    // Use to transform InducedSubgraph vertices to parent graph vertices
    virtual Vertex ParentGraph(Vertex v) const {
        return v;
    }
    virtual VertexPair ParentGraph(VertexPair uv) const {
        return uv;
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
    Weight FillCount(Vertex u, Vertex v) const {
        return G_->FillCount(U_[u], U_[v]);
    }
    Weight FillCount(VertexPair p) const {
        return G_->FillCount(U_[p.first], U_[p.second]);
    }
    Vertex ParentGraph(Vertex v) const {
        return U_[v];
    }
    VertexPair ParentGraph(VertexPair uv) const {
        return VertexPair(ParentGraph(uv.first), ParentGraph(uv.second));
    }

 protected:
    const Graph* const G_;
    const Vertices U_;
};  // InducedSubgraph

///////////
// Generics

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
