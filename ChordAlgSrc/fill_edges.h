/*
 * fill_edges.h - class used to represent a dynamic
 * set of fill edges of a graph.
 */

#ifndef CHORDALGSRC_FILL_EDGES_H_
#define CHORDALGSRC_FILL_EDGES_H_

#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class FillEdges {
 public:
    FillEdges() = delete;
    FillEdges(const FillEdges&) = delete;
    void operator=(const FillEdges&) = delete;

    explicit FillEdges(const Graph*);

    const Vertices& N(Vertex) const;
    bool IsEdge(VertexPair) const;      // is in V(G_) or F_
    bool IsEdge(Vertex, Vertex) const;
    bool IsFillEdge(VertexPair) const;  // is in F_
    bool IsFillEdge(Vertex, Vertex) const;
    bool IsClique(const Vertices&) const;
    bool AddEdge(VertexPair);           // new edge?
    bool AddEdge(Vertex, Vertex);
    void RemoveEdge(VertexPair);
    void RemoveEdge(Vertex, Vertex);
    void Saturate(const Vertices&);

    const VertexSet& operator[](size_t) const;
    VertexSet& operator[](size_t);
    size_t fill_count() const;
    Weight fill_weight() const;
    // Returns amalgamation of neighborhoods_ and G_->neighborhoods_
    AdjacencyLists* FilledNbhds() const;

 private:
    const Graph* const G_;

    std::vector< VertexSet > fill_;
    size_t fill_count_;
    Weight fill_weight_;
    AdjacencyLists* neighborhoods_;
};  // FillEdges

}  // namespace chordalg

#endif  // CHORDALGSRC_FILL_EDGES_H_
