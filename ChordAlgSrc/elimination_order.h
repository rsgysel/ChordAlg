/*
 *  elimination_order.h - data structure representing an elimination
 *  ordering of a graph
 */

#ifndef CHORDALGSRC_ELIMINATION_ORDER_H_
#define CHORDALGSRC_ELIMINATION_ORDER_H_

#include <cfloat>
#include <string>
#include <vector>

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
    void Init();

    void Emplace(Vertex, int);
    void Swap(int, int);
    AdjacencyLists* TriangNbhds() const;
    size_t ComputeFill();
    bool ZeroFill() const;

    std::string str() const;

    bool Before(Vertex u, Vertex v) const {
        return alpha_inverse_[u] < alpha_inverse_[v];
    }
    int PositionOf(Vertex v) const {
        return alpha_inverse_[v];
    }
    Vertex VertexAt(int i) const {
        return alpha_[i];
    }

    // neighbors of v ``left" (before) v. Unsorted.
    Vertices LNbhd(Vertex) const;
    // neighbors of v ``right" (after) v. Unsorted.
    Vertices RNbhd(Vertex) const;

    int fill_count() const {
        return fill_count_;
    }
    int size() const {
        return alpha_.size();
    }
    const Graph* G() const {
        return G_;
    }

    // Mutators
    void SetOrder(VertexVector);
    void SetPosition(Vertex v, int i) {
        alpha_inverse_[v] = i;
    }
    void SetVertex(int i, Vertex v) {
        alpha_[i] = v;
    }

 private:
    const Graph* const G_;

    Vertices alpha_;  // alpha[i] = ith vertex eliminated
    std::vector< int > alpha_inverse_;

    int fill_count_;  // # of fill edges added (monochromatic or not)

    std::vector< VertexSet > triangulation_nbhds_;

    static int kUnfilled() {
        return -1;
    }
};  // class EliminationOrder

}  // namespace chordalg

#endif  // CHORDALGSRC_ELIMINATION_ORDER_H_
