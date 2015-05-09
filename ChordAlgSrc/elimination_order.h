/*
 *  elimination_order.h - data structure representing an elimination
 *  ordering of a graph
 */

#ifndef CHORDALGSRC_ELIMINATION_ORDER_H_
#define CHORDALGSRC_ELIMINATION_ORDER_H_

#include <cfloat>
#include <string>
#include <vector>

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class EliminationOrder {
 public:
    EliminationOrder() = delete;
    EliminationOrder(const EliminationOrder&) = delete;
    void operator=(const EliminationOrder&) = delete;

    explicit EliminationOrder(const Graph*);
    ~EliminationOrder();

    static EliminationOrder* FromFile(const Graph*, std::string);
    void Init();
    void Emplace(Vertex, int);
    void Swap(int, int);
    FillEdges* ComputeFill() const;
    bool IsPerfect() const;

    bool Before(Vertex u, Vertex v) const;
    int PositionOf(Vertex v) const;
    Vertex VertexAt(int i) const;

    // neighbors of v ``left" (before) v. Unsorted.
    Vertices LNbhd(Vertex, const FillEdges* F = nullptr) const;
    // neighbors of v ``right" (after) v. Unsorted.
    Vertices RNbhd(Vertex, const FillEdges* F = nullptr) const;

    int size() const;
    const Graph* G() const;

    void SetOrder(VertexVector);
    void SetPosition(Vertex v, int i);
    void SetVertex(int i, Vertex v);

    std::string str() const;

 private:
    const Graph* const G_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< int > alpha_inverse_;
    bool filled_;
};  // class EliminationOrder

}  // namespace chordalg

#endif  // CHORDALGSRC_ELIMINATION_ORDER_H_
