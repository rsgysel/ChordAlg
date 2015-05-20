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
    const Triangulation* H,
    AdjacencyLists* E,
    CliqueMap* K) :
    H_(H),
    T_(new Tree(E, NamesFromCliqueMap(H, K))),
    K_(K) {
    return;
}

TreeRepresentation::~TreeRepresentation() {
    delete K_;
}

CliqueTree::CliqueTree(
    const Triangulation* H,
    AdjacencyLists* E,
    CliqueMap* K) :
    TreeRepresentation(H, E, K) {
    return;
}

CliqueTree::~CliqueTree() {
    return;
}

/////////////////////
// TreeRepresentation

const Triangulation& TreeRepresentation::H() const {
    return *H_;
}

const Tree& TreeRepresentation::T() const {
    return *T_;
}

const Vertices& TreeRepresentation::K(Vertex v) const {
    return (*K_)[v];
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
