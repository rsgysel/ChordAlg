/*
 *  tree_representation.h - a data structure for tree representations
 *  and clique trees
 */

#ifndef CHORDALGSRC_TREE_REPRESENTATION_H_
#define CHORDALGSRC_TREE_REPRESENTATION_H_

#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class TreeRepresentation {
 public:
    explicit TreeRepresentation(
        AdjacencyLists*,
        const Graph*,
        std::vector< Vertices >);
    virtual ~TreeRepresentation();

    std::string strNewick() const;
    std::string strPhyloNewick(
        const CellIntersectionGraph&,
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
    Graph T_;  // tree representation topology
    std::vector< Vertices > clique_map_;  // maps nodes of T_ to cliques of G_

    std::string strMaxClique(Vertices K) const;
    VertexNames NamesFromCliqueMap(std::vector< Vertices > clique_map) const;

    void NewickVisit(
        VertexSet& visited,
        Vertex v,
        std::string& newick_tree) const;
    void PhyloNewickVisit(
        VertexSet& visited,
        Vertex v,
        std::string& newick_tree,
        const CellIntersectionGraph& cig,
        std::vector< size_t >& taxon_clique_size) const;
};  // TreeRepresentation

class CliqueTree : public TreeRepresentation {
 public:
    explicit CliqueTree(
        AdjacencyLists*,
        const Graph*,
        std::vector< Vertices >);
    ~CliqueTree();
};  // CliqueTree

};  // namespace chordalg

#endif  // CHORDALGSRC_TREE_REPRESENTATION_H_
