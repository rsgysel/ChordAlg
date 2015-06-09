#include "ChordAlgSrc/potential_maxcliques.h"

#include "ChordAlgSrc/elimination.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace PotentialMaxcliques {

PMCTrie* Generate(const Graph& G) {
    // Graph, PMC and MS from last run
    Graph* Gi = nullptr;
    PMCTrie* Pi = nullptr;
    MinsepTrie* MSi = nullptr;
    // Graph, PMC and MS from new run (Gi+1 = Gi + Vertex i)
    Graph* Gi1 = nullptr;
    PMCTrie* Pi1 = nullptr;
    MinsepTrie* MSi1 = nullptr;
    EliminationOrder* eo = Elimination::ConnectedOrder(G);
    VertexSet V;
    // Start with Pi = {a}, MSi = {}, Gi = {v_0}
    V.insert(eo->VertexAt(0));
    Gi = new InducedSubgraph(&G, V);
    Pi = new PMCTrie(1);
    Vertices point = {0};
    Pi->Insert(point);
    MSi = new MinsepTrie(1);
    for (size_t i = 1; i < G.order(); ++i) {
        Vertex a = eo->VertexAt(i);
        V.insert(a);
        Gi1 = new InducedSubgraph(&G, V);
        MSi1 = MinimalSeparators::Generate(*Gi1);
        Pi1 = OneMoreVertex(*Gi, a, *Gi1, *Pi, *MSi, *MSi1);
        delete MSi;
        MSi = MSi1;
        delete Pi;
        Pi = Pi1;
        delete Gi;
        Gi = Gi1;
    }
    delete eo;
    delete Gi1;
    delete MSi1;
    return Pi1;
}

PMCTrie* OneMoreVertex(
    const Graph& Gi,
    Vertex a,
    const Graph& Gi1,
    const PMCTrie& Pi,
    const MinsepTrie& MSi,
    const MinsepTrie& MSi1) {
    PMCTrie* Pi1 = new PMCTrie(Gi1.order());
    // id of a in Gi1
    Vertex a_i1 = 0;
    for (Vertex v : Gi1) {
        if (Gi1.ParentGraph(v) < a) {
            ++a_i1;
        }
    }
    for (auto Omega : Pi) {
        // Translate Omega from Gi-vertices to Gi1-vertices
        for (Vertex& v : Omega) {
            if (Gi.ParentGraph(v) > a) {
                ++v;
            }
        }
        // Check if Omega or Omega' = Omega U {a} is a PMC
        if (IsPMC(Gi1, Vertices(Omega))) {
            Pi1->Insert(Omega);
        } else {
            Omega.push_back(a_i1);
            if (IsPMC(Gi1, Vertices(Omega))) {
                Pi1->Insert(Omega);
            }
        }
    }
    SeparatorBlocks B(&Gi1);
    for (auto S : MSi1) {
        // Translate S from Gi1-vertices to Gi-vertices
        bool a_in_S = false;
        Vertices Si(S);
        for (Vertex& v : Si) {
            if (Gi1.ParentGraph(v) > a) {
                --v;
            } else if (Gi1.ParentGraph(v) == a) {
                a_in_S = true;
            }
        }
        // Is there a T crossing S generating a new PMC?
        if (!a_in_S) {
            if (!MSi.Contains(Si)) {
                B.Separate(Vertices(S));
                for (auto T : MSi1) {
                    Vertices VT(T);
                    if (B.IsSeparated(VT)) {
                        for (Block b : B) {
                            if (b.IsFull()) {
                                Vertices Vint;
                                Vint.SetIntersection(VT, b.C());
                                Vertices K;
                                K.SetUnion(Vint, Vertices(S));
                                if (IsPMC(Gi1, K)) {
                                    Pi1->Insert(K);
                                }
                            }
                        }
                    }
                }
            }
            S.push_back(a_i1);
        }
        // Is S U {a} a PMC?
        if (IsPMC(Gi1, Vertices(S))) {
            Pi1->Insert(S);
        }
    }
    return Pi1;
}

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
