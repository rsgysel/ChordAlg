/*
 *  intersection_graph.h - an intersection graph data structure
 *  Copyright (C) 2013 Rob Gysel
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

#ifndef INCLUDE_INTERSECTION_GRAPH_H_
#define INCLUDE_INTERSECTION_GRAPH_H_

#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "graph.h"
#include "lex_trie.h"
#include "vertices.h"

namespace chordalg {

class ColoredIntersectionGraph : public Graph {
 public:
    explicit ColoredIntersectionGraph(MatrixCellIntGraphFR* M);
    explicit ColoredIntersectionGraph(NexusMRPFR* M);
    ColoredIntersectionGraph(ColoredIntersectionGraph&, Vertices);
    virtual ~ColoredIntersectionGraph();

    bool IsMonochromatic(Vertex, Vertex) const;
    size_t CommonColorCount(Vertex, Vertex) const;
    const Multicolor& vertex_color(Vertex v) const {
        return vertex_colors_[v];
    }
    Cost FillCost(Vertex u, Vertex v) const {
        return CommonColorCount(u, v);
    }
    Cost FillCost(VertexPair p) const {
        return FillCost(p.first, p.second);
    }

    std::string strSubsets();

    const LexTrie* subset_family() const {
        return subset_family_;
    }
    const FiniteSet& subset(Vertex v) const {
        return subsets_[v];
    }
    const std::string& taxon_name(size_t e) const {
        return taxon_name_[e];
    }
    size_t taxa() const {
        return taxon_name_.size();
    }

 private:
    std::vector< FiniteSet > subsets_;
    std::vector< Multicolor > vertex_colors_;
    LexTrie* subset_family_;
    std::vector< std::string > taxon_name_;

    std::vector< std::string > DefaultTaxonNames(size_t n);
    std::vector< FiniteSet > InduceSubsets(ColoredIntersectionGraph&,
                                        Vertices);
    std::vector< Multicolor > InduceVertexColors(ColoredIntersectionGraph&,
                                                 Vertices);
    LexTrie* InduceSubsetFamily(ColoredIntersectionGraph&);
};  // ColoredIntersectionGraph

}  // namespace chordalg

#endif  // INCLUDE_INTERSECTION_GRAPH_H_
