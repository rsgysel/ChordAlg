#include "tree_representation.h"

#include <string>
#include <vector>

namespace chordalg {

TreeRepresentation::TreeRepresentation(AdjacencyLists* E, Graph& G,
                                       std::vector< Vertices > clique_map) :
    G_(G),
    T_(E, NamesFromCliqueMap(clique_map)),
    clique_map_(clique_map) {
}

TreeRepresentation::~TreeRepresentation() {
}

std::string TreeRepresentation::SerializeMaxcliqueAsString(Vertices K) const {
    std::string maxclique("{");
    for (Vertex v : K) {
        maxclique += G_.name(v) + std::string(" ");
    }
    maxclique.erase(maxclique.end() - 1, maxclique.end());
    maxclique += std::string("}");
    return maxclique;
}

VertexNames TreeRepresentation::NamesFromCliqueMap(
    std::vector< Vertices > clique_map) const {
    VertexNames names;
    names.resize(clique_map.size());
    for (Vertex v = 0; v < clique_map.size(); ++v) {
        names[v] = SerializeMaxcliqueAsString(clique_map[v]);
    }
    return names;
}

CliqueTree::CliqueTree(AdjacencyLists* E, Graph& G,
                       std::vector<Vertices> clique_map) :
    TreeRepresentation(E, G, clique_map) {
}

CliqueTree::~CliqueTree() {
}

// Prints tree as a Newick notation / New Hampshire tree format.
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
void TreeRepresentation::NewickPrint() const {
    std::string newick_tree;
    VertexSet visited;
    Vertex v = *(T_.begin());
    visited.insert(v);
    NewickVisit(visited, v, newick_tree);
    if (visited.size() != T_.order()) {
        std::cerr << "Error in TreeRepresentation::NewickPrint: not all ";
        std::cerr << "nodes visited" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << newick_tree << ";" << std::endl;
    return;
}

void TreeRepresentation::NewickVisit(VertexSet& visited, Vertex v,
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
        newick_tree.erase(newick_tree.end() - 1);  // no () for leaf node
        newick_tree += subtree + T_.name(v);
    } else {
        subtree.erase(subtree.end() - 1);  // trailing comma
        newick_tree += subtree + std::string(")") + T_.name(v);
    }
    return;
}

void TreeRepresentation::PhyloNewickPrint(const ColoredIntersectionGraph& cig,
                                          bool rooted) const {
    // DFS info
    std::string newick_tree;
    VertexSet visited;
    Element root_element =  - 1;
    if (rooted) {
        for (int i = 0; i < cig.taxa(); ++i) {
            if (cig.taxon_name(i) == "roottaxon") {
                root_element = i;
            }
        }
        if (root_element ==  - 1) {
            std::cerr << "Error in TreeRepresentation::PhyloNewickPrint: no ";
            std::cerr << "roottaxon found" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    // Taxon Clique info
    std::vector< int > taxon_clique_size(cig.subset_family()->n(), 0);
    for (Vertex v : cig) {
        Subset S = cig.subset(v);
        for (Element e : S) {
            ++taxon_clique_size[e];
        }
    }
    Vertex v =  - 1;
    if (rooted) {
        for (Vertex u : T_) {
            int root_count = 0;
            for (Vertex w : clique_map_[u]) {
                for (Element e : cig.subset(w)) {
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
        if (v ==  - 1) {
            std::cerr << "Error in TreeRepresentation::PhyloNewickPrint: ";
            std::cerr << "no roottaxon clique found" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        v = *(T_.begin());
        visited.insert(v);
    }
    // Start DFS
    PhyloNewickVisit(visited, v, newick_tree, cig, taxon_clique_size);
    if (visited.size() != T_.order()) {
        std::cerr << "Error in TreeRepresentation::PhyloNewickPrint: not ";
        std::cerr << "all nodes visited" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << newick_tree << ";" << std::endl;
    return;
}

void TreeRepresentation::PhyloNewickVisit(VertexSet& visited,
        Vertex v,
        std::string& newick_tree,
        const ColoredIntersectionGraph& cig,
        std::vector< int >& taxon_clique_size) const {
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
    std::vector< int > cell_count(taxon_clique_size.size(), 0);
    for (Vertex v : clique_map_[v]) {
        for (Element e : cig.subset(v)) {
            ++cell_count[e];
        }
    }
    std::vector<std::string> node_taxa;
    int roottaxon = 0;
    for (int i = 0; i < cell_count.size(); ++i) {
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
        node_name.erase(node_name.end() - 1);
        node_name += ")";
    }
    // Leaf or Internal node
    if (is_leaf) {
        if (node_taxa.size() == 1) {
            node_name = node_taxa[0];
        }
        newick_tree.erase(newick_tree.end() - 1);   // no () for leaf node
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
