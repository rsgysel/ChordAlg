#ifndef CHORDALGSRC_FILL_EDGES_H_
#define CHORDALGSRC_FILL_EDGES_H_

#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class FillEdges : public std::vector< VertexSet > {
 public:
    FillEdges() = delete;
    FillEdges(const FillEdges&) = delete;
    void operator=(const FillEdges&) = delete;

    FillEdges(const Graph*);

    bool IsEdge(VertexPair) const;      // is in V(G_) or F_
    bool IsEdge(Vertex, Vertex) const;
    bool IsFillEdge(VertexPair) const;  // is in F_
    bool IsFillEdge(Vertex, Vertex) const;
    bool AddEdge(VertexPair);           // new edge?
    bool AddEdge(Vertex, Vertex);
    void Saturate(Vertices);

    size_t fill_count() const;
    Weight fill_weight() const;

    AdjacencyLists* TriangulationNbhds() const;

 private:
    const Graph* const G_;

    size_t fill_count_;
    Weight fill_weight_;
};  // FillEdges

}  // namespace chordalg

#endif  // CHORDALGSRC_FILL_EDGES_H_
