#include "minimal_separator_algorithms.h"

namespace chordalg {

std::string strMinimalSeparators(const MinsepTrie& M, const Graph& G) {
    std::string MSstr;
    for (FiniteSet S : M) {
        MSstr += G.str(S) + '\n';
    }
    return MSstr;
}

// Berry, Bordat, and Cogis' algorithm to generate minimal separators
// paper: http://www.worldscientific.com/doi/pdf/10.1142/S0129054100000211
MinsepTrie* MinimalSeparators(const Graph& G) {
    bool new_set;
    int n = G.order();
    MinsepTrie* M = new MinsepTrie(n);
    // minimal separators not yet processed. entries are sorted
    MinsepStack M_stack;
    // current vertex separator
    SeparatorBlocks S(&G);
    // first phase: the neighborhood of each connected component
    // of G - N[v] for each vertex v is a minimal separator
    for (Vertex v : G) {
        S.SeparateClosedNbhd(v);
        for (Block B : S) {
            M->SortedInsert< Vertices >(B.NC(), &new_set);
            if (new_set) {
                M_stack.push(B.NC());
            }
        }
    }
    // second phase: for each v in S, the neighborhood of each
    // connected components of G - S U N[v] is a minimal separator
    Vertices V(n);
    while (!M_stack.empty()) {
        Vertices U = M_stack.top();
        M_stack.pop();
        for (Vertex v : U) {
            V.merge(U, G.N(v));
            S.Separate(V);
            for (Block B : S) {
                M->SortedInsert< Vertices >(B.NC(), &new_set);
                if (new_set) {
                    M_stack.push(B.NC());
                }
            }
        }
    }
    return M;
}

MinsepTrie* MinimalSeparators(const Graph& G, Vertex a, Vertex b) {
    bool new_set;
    int n = G.order();
    MinsepTrie* M = new MinsepTrie(n);
    // minimal separators not yet processed. entries are sorted
    MinsepStack M_stack;
    // current vertex separator
    Vertices U(n);
    SeparatorBlocks S(&G);
    // first phase: the neighborhood of the connected component
    // of G - N[a] containing b is a minimal ab-separator
    S.SeparateClosedNbhd(a);
    if (!S.IsInSeparator(b)) {
        M->SortedInsert< Vertices >(S.NComponentOf(b));
        M_stack.push(S.NComponentOf(b));
    }
    // second phase: for each v in S, the neighborhood of each
    // connected components of G - S U N[v] is a minimal separator
    Vertices V(n);
    while (!M_stack.empty()) {
        Vertices U = M_stack.top();
        M_stack.pop();
        for (Vertex v : U) {
            V.merge(U, G.N(v));
            S.Separate(V);
            if (!S.IsInSeparator(b) && !S.NComponentOf(b).empty()) {
                M->SortedInsert<Vertices>(S.NComponentOf(b), &new_set);
                if (new_set) {
                    M_stack.push(S.NComponentOf(b));
                }
            }
        }
    }
    return M;
}

}  // namespace chordalg
