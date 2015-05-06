/*
 *  graph.h - a graph data structure
 */

#ifndef CHORDALGSRC_GRAPH_H_
#define CHORDALGSRC_GRAPH_H_

#include <string>
#include <vector>

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class Graph {
 public:
    Graph() = delete;
    Graph(const Graph&) = delete;
    void operator=(const Graph&) = delete;

    explicit Graph(Graph&);
    explicit Graph(GraphFR*);
    explicit Graph(AdjacencyLists*);
    Graph(AdjacencyLists*, VertexNames);
    explicit Graph(size_t);
    virtual ~Graph();

    static Graph* New(std::string);
    static Graph* New(GraphFile*);

    bool IsIsomorphic(Graph&) const;
    virtual std::string str() const;
    std::string str(const LexTrie&) const;
    std::string str(const VertexVector&) const;
    std::string str(const Vertices&) const;

    Vertices V() const;
    GraphVertexIterator begin() const;
    GraphVertexIterator end() const;

    const AdjacencyLists& neighbors() const;
    virtual VertexName name(Vertex v) const;
    size_t order() const;
    size_t size() const;
    virtual bool HasEdge(Vertex u, Vertex v) const;
    virtual bool HasEdge(VertexPair uv) const;
    virtual Weight FillCount(Vertex u, Vertex v) const;
    virtual Weight FillCount(VertexPair uv) const;
    bool IsClique() const;
    const Vertices& N(Vertex v) const;

    // Use to transform InducedSubgraph vertices to parent graph vertices
    virtual Vertex ParentGraph(Vertex v) const;
    virtual VertexPair ParentGraph(VertexPair uv) const;

    template< class Container >
    bool HasClique(Container set) const;
 protected:
    void Init();
    // Induced Subgraph Initialization
    AdjacencyLists* InducedVertices(const Graph&, Vertices);
    VertexNames* DefaultNames(size_t);

    const AdjacencyLists* const neighborhoods_;
    const VertexNames* const vertex_names_;
    std::vector< std::vector< bool > > is_edge_;
    size_t order_;  // #vertices
    size_t size_;  // #edges

    template< class InputIterator >
    bool HasClique(InputIterator begin, InputIterator end) const;
};  // Graph

class InducedSubgraph : public Graph {
 public:
    InducedSubgraph() = delete;
    InducedSubgraph(const InducedSubgraph&) = delete;
    void operator=(const InducedSubgraph&) = delete;

    InducedSubgraph(const Graph*, Vertices);

    VertexName name(Vertex v) const;
    Weight FillCount(Vertex u, Vertex v) const;
    Weight FillCount(VertexPair uv) const;
    Vertex ParentGraph(Vertex v) const;
    VertexPair ParentGraph(VertexPair uv) const;

 protected:
    const Graph* const G_;
    const Vertices U_;
};  // InducedSubgraph

///////////
// Generics

template< class Container >
bool Graph::HasClique(Container set) const {
    return HasClique(set.begin(), set.end());
}

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

#endif  // CHORDALGSRC_GRAPH_H_
