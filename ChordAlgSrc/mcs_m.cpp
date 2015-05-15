#include "ChordAlgSrc/mcs_m.h"

#include <algorithm>
#include <list>
#include <vector>

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace MCSmPlus {

// Implementation of MCS-M+, which minimally triangulates a graph.
// Described for atom computation by Berry, Pogorelcnik, and Simonet.
// paper: http://www.mdpi.com/1999-4893/3/2/197
//
void Run(const Graph* G,
         EliminationOrder* eo,
         FillEdges* F,
         VertexList* minsep_generators) {
    int s = -1;  // as in paper: for finding minimal separator generators
    size_t n = G->order();
    std::vector< int > label;  // as in paper
    label.resize(n);
    for (Vertex v : label) {
        label[v] = 0;
    }
    const int kDeleted = -1;  // for deleted vertices
    std::vector< bool > reached;  // as in paper
    reached.resize(n);
    for (int i = n - 1; i >= 0; --i) {
        auto max_itr = std::max_element(label.begin(), label.end());
        int max_label = *max_itr;
        Vertex x = std::distance(label.begin(), max_itr);
        if (max_label <= s && minsep_generators) {
            minsep_generators->push_front(i);
        }
        s = max_label;
        std::list< int > Y;  // as in paper
        label[x] = kDeleted;
        for (Vertex v : *G) {
            reached[v] = false;
        }
        reached[x] = true;
        std::vector< std::list< Vertex > > reach;
        reach.resize(n);
        for (Vertex y : G->N(x)) {
            if (label[y] != kDeleted) {
                reached[y] = true;
                reach[ label[y] ].push_back(y);
                Y.push_back(y);
            }
        }
        for (size_t j = 0; j < n - 1; ++j) {
            while (!reach[j].empty()) {
                Vertex y = reach[j].back();
                reach[j].pop_back();
                for (Vertex z : G->N(y)) {
                    if (label[z] != kDeleted && !reached[z]) {
                        reached[z] = true;
                        reach[ label[z] ].push_back(z);
                        if (label[z] > static_cast<int>(j)) {
                            Y.push_back(z);
                            reach[ label[z] ].push_back(z);
                        } else {
                            reach[j].push_back(z);
                        }
                    }
                }
            }
        }
        for (Vertex y : Y) {
            label[y]++;
            F->AddEdge(x, y);
        }
        eo->Emplace(x, i);
    }
    return;
}

}  // namespace MCSmPlus

}  // namespace chordalg

