#include "ChordAlgSrc/tree.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Tree::Tree(AdjacencyLists* E) : Graph(E) {
    return;
}

Tree::Tree(AdjacencyLists* E, VertexNames N) : Graph(E, N) {
    return;
}

///////
// Tree

bool Tree::IsTree() const {
    if (order_ != size_ + 1) {
        return false;
    }
    SeparatorComponents S(this);
    Vertices V;
    S.Separate(V);
    return S.size() == 1;
}

// Prints tree in Newick notation / New Hampshire tree format.
// ``string" may not contain semicolon, parentheses, comma, or colon
//
// Newick files are given by the grammar:
//   Tree --> Subtree ";"
//   Subtree --> Leaf | Internal
//   Leaf --> Name
//   Internal --> "(" BranchList ")" Name
//   BranchList --> Branch | Branch "," BranchList
//   Branch --> Subtree Length
//   Name --> empty | string
//   Length --> empty | ":" number
//
std::string Tree::strNewick() const {
    return strNewick(0);
}

std::string Tree::strNewick(Vertex v) const {
    std::string str;
    VertexSet* visited = new VertexSet();
    str += NewickVisit(v, visited);
    str += ";";
    delete visited;
    return str;
}

std::string Tree::NewickVisit(Vertex v, VertexSet* visited) const {
    std::string str;
    visited->insert(v);
    bool is_leaf = true;
    std::string subtree;
    for (Vertex u : N(v)) {
        if (visited->find(u) == visited->end()) {
            is_leaf = false;
            subtree += NewickVisit(u, visited);
            subtree += ",";
        }
    }
    if (!is_leaf) {
        subtree.pop_back();     // remove trailing comma
        str += std::string("(") + subtree + std::string(")");
    } else {
        str += subtree;
    }
    str += name(v);
    return str;
}

}  // namespace chordalg
