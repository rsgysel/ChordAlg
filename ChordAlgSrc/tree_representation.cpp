#include "ChordAlgSrc/tree_representation.h"

#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/tree.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

TreeRepresentation::TreeRepresentation(
    const Graph* G,
    AdjacencyLists* E,
    CliqueMap* K) :
    G_(G),
    T_(new Tree(E, NamesFromCliqueMap(G, K))),
    K_(K) {
    return;
}

TreeRepresentation::~TreeRepresentation() {
    delete K_;
}

CliqueTree::CliqueTree(
    const Graph* G,
    AdjacencyLists* E,
    CliqueMap* K) :
    TreeRepresentation(G, E, K) {
    return;
}

CliqueTree::~CliqueTree() {
    return;
}

/////////////////////
// TreeRepresentation

const Graph& TreeRepresentation::G() const {
    return *G_;
}

const Tree& TreeRepresentation::T() const {
    return *T_;
}

std::string TreeRepresentation::str() const {
    return T_->str();
}

VertexNames TreeRepresentation::NamesFromCliqueMap(
    const Graph* G,
    const CliqueMap* K) const {
    VertexNames names;
    names.resize(K->size());
    for (Vertex v = 0; v < K->size(); ++v) {
        std::string maxclique("{");
        for (Vertex u : (*K)[v]) {
            maxclique += G->name(u) + std::string(",");
        }
        if (!(*K)[v].empty()) {
            maxclique.pop_back();
        }
        maxclique += std::string("}");
        names[v] = maxclique;
    }
    return names;
}

}  // namespace chordalg
