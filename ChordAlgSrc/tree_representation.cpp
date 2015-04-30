#include "ChordAlgSrc/tree_representation.h"

#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

TreeRepresentation::TreeRepresentation(
    AdjacencyLists* E,
    const Graph* G,
    std::vector< Vertices > clique_map) :
    G_(G),
    T_(E, NamesFromCliqueMap(clique_map)),
    clique_map_(clique_map) {
}

TreeRepresentation::~TreeRepresentation() {
}

std::string TreeRepresentation::strMaxClique(Vertices K) const {
    std::string maxclique("{");
    maxclique += G_->str(K);
    maxclique += std::string("}");
    return maxclique;
}

VertexNames TreeRepresentation::NamesFromCliqueMap(
    std::vector< Vertices > clique_map) const {
    VertexNames names;
    names.resize(clique_map.size());
    for (Vertex v = 0; v < clique_map.size(); ++v) {
        names[v] = strMaxClique(clique_map[v]);
    }
    return names;
}

CliqueTree::CliqueTree(
    AdjacencyLists* E,
    const Graph* G,
    std::vector<Vertices> clique_map) :
    TreeRepresentation(E, G, clique_map) {
}

CliqueTree::~CliqueTree() {
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
std::string TreeRepresentation::strNewick() const {
    std::string newick_tree;
    VertexSet visited;
    Vertex v = *(T_.begin());
    visited.insert(v);
    NewickVisit(visited, v, newick_tree);
    newick_tree += ";";
    return newick_tree;
}

void TreeRepresentation::NewickVisit(
    VertexSet& visited,
    Vertex v,
    std::string& newick_tree) const {
    visited.insert(v);
    bool is_leaf = true;
    newick_tree += std::string("(");
    std::string subtree;
    for (Vertex u : T_.N(v)) {
        if (visited.find(u) == visited.end()) {
            is_leaf = false;
            NewickVisit(visited, u, subtree);
            subtree += std::string(",");
        }
    }
    if (is_leaf) {
        newick_tree.pop_back();  // no () for leaf node
        newick_tree += subtree + T_.name(v);
    } else {
        subtree.pop_back();  // trailing comma
        newick_tree += subtree + std::string(")") + T_.name(v);
    }
    return;
}

std::string TreeRepresentation::strPhyloNewick(
    const CellIntersectionGraph& cig,
    bool rooted) const {
    // DFS info
    std::string newick_tree;
    VertexSet visited;
    size_t root_element = SIZE_MAX;
    if (rooted) {
        for (size_t i = 0; i < cig.taxa(); ++i) {
            if (cig.taxon_name(i) == "roottaxon") {
                root_element = i;
            }
        }
    }
    // Taxon Clique info
    std::vector< size_t > taxon_clique_size(cig.subset_family()->n(), 0);
    for (Vertex v : cig) {
        FiniteSet S = cig.subset(v);
        for (size_t t : S) {
            ++taxon_clique_size[t];
        }
    }
    Vertex v;
    if (rooted) {
        for (Vertex u : T_) {
            size_t root_count = 0;
            for (Vertex w : clique_map_[u]) {
                for (size_t e : cig.subset(w)) {
                    if (e == root_element) {
                        ++root_count;
                    }
                }
            }
            if (root_count == taxon_clique_size[root_element]) {
                v = u;
                break;
            }
        }
    } else {
        v = *(T_.begin());
        visited.insert(v);
    }
    // Start DFS
    PhyloNewickVisit(visited, v, newick_tree, cig, taxon_clique_size);
    newick_tree += ";";
    return newick_tree;
}

void TreeRepresentation::PhyloNewickVisit(
    VertexSet& visited,
    Vertex v,
    std::string& newick_tree,
    const CellIntersectionGraph& cig,
    std::vector< size_t >& taxon_clique_size) const {
    visited.insert(v);
    bool is_leaf = true;
    newick_tree += std::string("(");
    std::string subtree;
    // Recurse
    for (Vertex u : T_.N(v)) {
        if (visited.find(u) == visited.end()) {
            is_leaf = false;
            PhyloNewickVisit(visited, u, subtree, cig, taxon_clique_size);
            subtree += std::string(",");
        }
    }
    // Calculate candidate-ness of node
    std::vector< size_t > cell_count(taxon_clique_size.size(), 0);
    for (Vertex u : clique_map_[v]) {
        for (size_t e : cig.subset(u)) {
            ++cell_count[e];
        }
    }
    std::vector<std::string> node_taxa;
    size_t roottaxon = 0;
    for (size_t i = 0; i < cell_count.size(); ++i) {
        if (cell_count[i] == taxon_clique_size[i]) {
            node_taxa.push_back(cig.taxon_name(i));
            if (cig.taxon_name(i) == "roottaxon") {
                roottaxon = 1;
            }
        }
    }
    std::string node_name;
    if (node_taxa.size() - roottaxon > 0) {
        if (!is_leaf) {
            node_name += ",";
        }
        node_name += "(";
        for (std::string taxon : node_taxa) {
            if (taxon != "roottaxon") {
                node_name += taxon += ",";
            }
        }
        node_name.pop_back();
        node_name += ")";
    }
    // Leaf or Internal node
    if (is_leaf) {
        if (node_taxa.size() == 1) {
            node_name = node_taxa[0];
        }
        newick_tree.pop_back();   // no () for leaf node
        newick_tree += subtree + node_name;
    } else {
        subtree.erase(subtree.end() - 1);    // trailing comma
        newick_tree += subtree + node_name + std::string(")");
        if (roottaxon) {
            newick_tree += std::string("roottaxon");
        }
    }
    return;
}

};  // namespace chordalg
