#include "ChordAlgSrc/supertree.h"

#include <map>
#include <string>
#include <utility>

#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/tree.h"
#include "ChordAlgSrc/tree_representation.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace SuperTree {

VertexNames* TaxaNames(const CliqueTree&, const CellIntersectionGraph&);

std::string NewickStr(const CliqueTree& CT, const CellIntersectionGraph& G) {
    if (&CT.H().G() != &G) {
        std::cerr << "Error in SuperTree::NewickStr:"
                  << " not a clique-tree of given graph\n";
        exit(EXIT_FAILURE);
    }
    const Tree& T = CT.T();
    AdjacencyLists* neighborhoods = new AdjacencyLists(T.neighborhoods());
    VertexNames* names = TaxaNames(CT, G);
    Tree* P = new Tree(neighborhoods, *names);
    std::string result = P->strNewick();
    delete P;
    return result;
}

VertexNames* TaxaNames(const CliqueTree& CT, const CellIntersectionGraph& G) {
    VertexNames* names = new VertexNames(G.taxa());
    // u, v denotes nodes of CT.T, A, B denotes vertices of G
    for (Vertex v : CT.T()) {
        // Compute size of taxon-cliques contained in K(v)
        std::map< size_t, size_t > clique_size;
        for (Vertex A : CT.K(v)) {
            for (auto t : G.subset(A)) {
                ++(*clique_size.emplace(t, 0).first).second;
            }
        }
        // Add taxon names if entire taxon-clique in K(v)
        for (std::pair< size_t, size_t > p : clique_size) {
            if (G.taxon_clique(p.first).size() == p.second) {
                (*names)[v] += G.taxon_name(p.first) + std::string("__");
            }
        }
        if ((*names)[v].size() > 0) {
            (*names)[v].pop_back();
            (*names)[v].pop_back();
        }
    }
    return names;
}

}  // namespace SuperTree

}  // namespace chordalg
