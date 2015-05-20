#include "ChordAlgSrc/elimination.h"

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace Elimination {

// Tarjan and Yannakakis' algorithm to compute fill edges from an elimination
// order, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test
// chordality of graphs, test acyclicity of hypergraphs, and selectively
// reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
AdjacencyLists* FillIn(const Graph& G, const EliminationOrder& eo) {
    FillEdges F(&G);
    VertexVector follower;
    follower.resize(G.order());
    VertexVector index;
    index.resize(G.order());
    for (size_t i = 0; i < G.order(); ++i) {
        Vertex w = eo.VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for (Vertex v : eo.LNbhd(w, &F)) {
            Vertex x = v;
            while (index[x] < i) {
                index[x] = i;
                F.AddEdge(x, w);
                x = follower[x];
            }
            if (follower[x] == x) {
                follower[x] = w;
            }
        }
    }
    return F.FilledNbhds();
}

// Tarjan and Yannakakis' algorithm to check if an elimination order is
// perfect, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test
// chordality of graphs, test acyclicity of hypergraphs, and selectively
// reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
bool ZeroFill(const Graph& G, const EliminationOrder& eo) {
    VertexVector follower;
    follower.resize(G.order());
    VertexVector index;
    index.resize(G.order());
    for (size_t i = 0; i < G.order(); ++i) {
        Vertex w = eo.VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for (Vertex v : eo.LNbhd(w)) {
            index[v] = i;
            if (follower[v] == v) {
                follower[v] = w;
            }
        }
        for (Vertex v : eo.LNbhd(w)) {
            if (index[follower[v]] < i) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace Elimination

}  // namespace chordalg
