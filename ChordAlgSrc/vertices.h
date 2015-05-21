/*
 *  vertices.h - iterators, pairs, and typedefs
 */

#ifndef CHORDALGSRC_VERTICES_H_
#define CHORDALGSRC_VERTICES_H_

#include <algorithm>
#include <initializer_list>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <utility>

namespace chordalg {

// Forward declarations
class                                   Graph;
class                                   Vertices;

// Types

typedef size_t                          Vertex;
typedef std::pair< Vertex, Vertex >     VertexPair;

typedef std::list   < Vertex >          VertexList;
typedef std::queue  < Vertex >          VertexQueue;
typedef std::set    < Vertex >          VertexSet;
typedef std::vector < Vertex >          VertexVector;

typedef Vertices                        Nbhd;
typedef std::vector< Nbhd >             AdjacencyLists;

typedef std::string                     VertexName;
typedef std::vector< VertexName >       VertexNames;

typedef size_t                          Color;
typedef std::list< Color >              Multicolor;

// elimination ordering typedefs
typedef double                          Weight;         // weight of fill edge
typedef std::pair< Vertex, Weight >     VertexWeight;

class Vertices : public VertexVector {
 public:
    Vertices();
    explicit Vertices(size_t);
    explicit Vertices(const VertexList&);
    explicit Vertices(const VertexSet&);
    explicit Vertices(const VertexVector&);
    Vertices(const std::initializer_list<Vertex>&);

    void sort();
    // These functions require sorted inputs
    void set_intersection(const Vertices&, const Vertices&);
    void set_union(const Vertices&, const Vertices&);

    std::string str() const;
};  // Vertices

class GraphVertexIterator {
 public:
    explicit GraphVertexIterator(const Graph*);
    explicit GraphVertexIterator(const Graph*, Vertex);

    void operator++();
    Vertex operator*() const;
    bool operator!=(const GraphVertexIterator&) const;
    bool operator==(const GraphVertexIterator&) const;

 private:
    const Graph* const G_;
    Vertex v_;
};  // GraphVertexIterator

class GraphVertices : public Vertices {
 public:
    explicit GraphVertices(const Graph*, size_t);

    GraphVertexIterator begin() const;
    GraphVertexIterator end() const;

 private:
    const Graph* const G_;
    size_t order_;
};  // GraphVertices

class VertexPairs;

class VertexPairsIterator {
 public:
    explicit VertexPairsIterator(const Vertices*, Vertex, Vertex);

    void operator++();
    bool operator==(const VertexPairsIterator&) const;
    bool operator!=(const VertexPairsIterator&) const;
    VertexPair operator*();

 private:
    const Vertices* const V_;
    Vertex v1_, v2_;
};  // VertexPairsIterator

class VertexPairs {
 public:
    explicit VertexPairs(Vertices);

    VertexPairsIterator begin() const;
    VertexPairsIterator end() const;

 private:
    const Vertices V_;
    size_t begin_, end_;
};  // VertexPairs

}  // namespace chordalg

#endif  // CHORDALGSRC_VERTICES_H_
