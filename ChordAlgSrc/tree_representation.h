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

#include <string>
#include <vector>

#include "graph.h"
#include "intersection_graph.h"

namespace chordalg {

class TreeRepresentation {
 public:
    TreeRepresentation(AdjacencyLists* E, Graph& G,
                       std::vector< Vertices > clique_map);
    virtual ~TreeRepresentation();

    void NewickVisit(VertexSet& visited,
                     Vertex v,
                     std::string& newick_tree) const;
    void PhyloNewickVisit(VertexSet& visited,
                          Vertex v,
                          std::string& newick_tree,
                          const ColoredIntersectionGraph& cig,
                          std::vector< size_t >& taxon_clique_size) const;


    void NewickPrint() const;
    void PhyloNewickPrint(const ColoredIntersectionGraph& cig,
                          bool rooted = false) const;
    void PrettyPrint() const {
        T_.PrettyPrint();
    }

    const Graph& G() const {
        return G_;
    }
    const Graph& T() const {
        return T_;
    }

 protected:
    Graph& G_;  // underlying chordal graph
    Graph T_;  // topology
    std::vector< Vertices > clique_map_;  // maps nodes of T_ to cliques of G_

    std::string SerializeMaxcliqueAsString(Vertices K) const;
    VertexNames NamesFromCliqueMap(std::vector< Vertices > clique_map) const;
};  // TreeRepresentation

class CliqueTree : public TreeRepresentation {
 public:
    CliqueTree(AdjacencyLists* E, Graph& G, std::vector< Vertices > clique_map);
    ~CliqueTree();
};  // CliqueTree

};  // namespace chordalg

#endif  // INCLUDE_TREE_REPRESENTATION_H_
