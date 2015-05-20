#include "ChordAlgSrc/lb_triang.h"

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/triangulation.h"

namespace chordalg {

namespace LBTriang {

Triangulation* Run(const Graph* G, const EliminationOrder* eo) {
    const EliminationOrder* pi = eo ? eo : new EliminationOrder(G);
    FillEdges F(G);
    SeparatorBlocks S(G);
    for (size_t i = 0; i < G->order(); ++i) {
        Vertex v = pi->VertexAt(i);
        S.SeparateClosedNbhd(v, &F);
        for (Block B : S) {
            F.Saturate(B.NC());
        }
    }
    Triangulation* H = Triangulation::New(G, F);
    if (!eo) {
        delete pi;
    }
    return H;
}

}  // namespace LBTriang

}  // namespace chordalg
