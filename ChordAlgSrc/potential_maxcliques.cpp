#include "ChordAlgSrc/potential_maxcliques.h"

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace PotentialMaxcliques {

bool IsPMC(const Graph& G, const Vertices& K) {
    FillEdges F(&G);
    SeparatorBlocks S(&G);
    S.Separate(K, &F);
    for (Block B : S) {
        if (B.IsFull()) {
            return false;
        }
    }
    for (Block B : S) {
        F.Saturate(B.NC());
    }
    return F.IsClique(K);
}

}  // namespace PotentialMaxcliques

}  // namespace chordalg
