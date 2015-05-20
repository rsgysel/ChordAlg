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

class EliminationOrder;

class Graph {
 public:
    Graph() = delete;
    void operator=(const Graph&) = delete;

    Graph(const Graph&);
    explicit Graph(GraphFR*);
    explicit Graph(AdjacencyLists*);
    Graph(AdjacencyLists*, VertexNames);
    explicit Graph(size_t);
    virtual ~Graph();

    static Graph* New(std::string);
    static Graph* New(GraphFile*);

    bool IsIsomorphic(const Graph&) const;
    bool IsSupergraph(const Graph&) const;
    virtual std::string str() const;
    std::string str(const LexTrie&) const;
    std::string str(const VertexVector&) const;
    std::string str(const Vertices&) const;
    std::string strDOT(const std::string&) const;
    std::string strGML(const std::string&) const;

    Vertices V() const;
    GraphVertexIterator begin() const;
    GraphVertexIterator end() const;

    const AdjacencyLists& neighborhoods() const;
    virtual VertexName name(Vertex v) const;
    size_t order() const;
    size_t size() const;
    virtual bool IsEdge(Vertex u, Vertex v) const;
    virtual bool IsEdge(VertexPair uv) const;
    virtual Weight FillCount(Vertex u, Vertex v) const;
    virtual Weight FillCount(VertexPair uv) const;
    bool IsClique() const;
    bool IsClique(const Vertices& set) const;
    const Vertices& N(Vertex v) const;

    // Use to transform InducedSubgraph vertices to parent graph vertices
    virtual Vertex ParentGraph(Vertex v) const;
    virtual VertexPair ParentGraph(VertexPair uv) const;

 protected:
    void Init();
    // Induced Subgraph Initialization
    static AdjacencyLists* InducedVertices(const Graph&, const Vertices&);
    static VertexNames* DefaultNames(size_t);

    const AdjacencyLists* const neighborhoods_;
    const VertexNames* const vertex_names_;
    std::vector< std::vector< bool > > is_edge_;
    size_t order_;  // #vertices
    size_t size_;  // #edges
};  // Graph

class InducedSubgraph : public Graph {
 public:
    InducedSubgraph() = delete;
    InducedSubgraph(const InducedSubgraph&) = delete;
    void operator=(const InducedSubgraph&) = delete;

    InducedSubgraph(const Graph*, const Vertices*);

    VertexName name(Vertex v) const;
    Weight FillCount(Vertex u, Vertex v) const;
    Weight FillCount(VertexPair uv) const;
    Vertex ParentGraph(Vertex v) const;
    VertexPair ParentGraph(VertexPair uv) const;

 protected:
    const Graph* const G_;
    const Vertices U_;
};  // InducedSubgraph

}  // namespace chordalg

#endif  // CHORDALGSRC_GRAPH_H_
