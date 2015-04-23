/*
 *  tree_representation.h - a data structure for tree representations
 *  and clique trees
 */

#ifndef INCLUDE_TREE_REPRESENTATION_H_
#define INCLUDE_TREE_REPRESENTATION_H_

#include <sstream>
#include <string>
#include <vector>

#include "graph.h"
#include "intersection_graph.h"

namespace chordalg {

class TreeRepresentation {
 public:
    explicit TreeRepresentation(AdjacencyLists* E, const Graph* G,
                                std::vector< Vertices > clique_map);
    virtual ~TreeRepresentation();

    std::string strNewick() const;
    std::string strPhyloNewick(const CellIntersectionGraph& cig,
                               bool rooted = false) const;
    std::string str() const {
        return T_.str();
    }

    const Graph* G() const {
        return G_;
    }
    const Graph& T() const {
        return T_;
    }

 protected:
    const Graph* G_;  // represented chordal graph
    Graph T_;  // topology
    std::vector< Vertices > clique_map_;  // maps nodes of T_ to cliques of G_

    std::string strMaxClique(Vertices K) const;
    VertexNames NamesFromCliqueMap(std::vector< Vertices > clique_map) const;

    void NewickVisit(VertexSet& visited,
                     Vertex v,
                     std::string& newick_tree) const;
    void PhyloNewickVisit(VertexSet& visited,
                          Vertex v,
                          std::string& newick_tree,
                          const CellIntersectionGraph& cig,
                          std::vector< size_t >& taxon_clique_size) const;
};  // TreeRepresentation

class CliqueTree : public TreeRepresentation {
 public:
    CliqueTree(AdjacencyLists* E, const Graph* G, std::vector< Vertices > clique_map);
    ~CliqueTree();
};  // CliqueTree

};  // namespace chordalg

#endif  // INCLUDE_TREE_REPRESENTATION_H_
