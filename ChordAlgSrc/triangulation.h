#ifndef CHORDALGSRC_TRIANGULATION_H_
#define CHORDALGSRC_TRIANGULATION_H_

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/heuristic_run.h"

namespace chordalg {

class Triangulation : public Graph {
 public:
    Triangulation() = delete;
    Triangulation(const Triangulation&) = delete;
    void operator=(const Triangulation&) = delete;

    explicit Triangulation(const Graph*, const FillEdges*);
    explicit Triangulation(const Graph*, GraphFR*);

    static Triangulation* New(const Graph*, const EliminationOrder*);
    static Triangulation* New(const Graph*, const HeuristicRun*);
    static Triangulation* New(const Graph*, const std::string);
    VertexName name(Vertex v) const;
    bool IsFillEdge(Vertex, Vertex) const;
    bool IsFillEdge(VertexPair) const;
    bool IsChordal() const;
    static bool IsChordal(const Graph*);
    bool IsMinimalTriangulation() const;
    FillEdges* CopyFill() const;

 protected:
    const Graph* const G_;
};  // Triangulation

}  // namespace chordalg

#endif  // CHORDALGSRC_TRIANGULATION_H_
