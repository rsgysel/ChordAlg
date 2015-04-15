/*
 *  tree_representation.h - a data structure for tree representations
 *  and clique trees
 *  Copyright (C) 2014 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    std::string strPhyloNewick(const ColoredIntersectionGraph& cig,
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
                          const ColoredIntersectionGraph& cig,
                          std::vector< size_t >& taxon_clique_size) const;
};  // TreeRepresentation

class CliqueTree : public TreeRepresentation {
 public:
    CliqueTree(AdjacencyLists* E, const Graph* G, std::vector< Vertices > clique_map);
    ~CliqueTree();
};  // CliqueTree

};  // namespace chordalg

#endif  // INCLUDE_TREE_REPRESENTATION_H_
