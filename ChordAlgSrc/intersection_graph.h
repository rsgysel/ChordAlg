/*
 *  intersection_graph.h - an intersection graph data structure
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

class CellIntersectionGraph : public Graph {
 public:
    CellIntersectionGraph() = delete;
    CellIntersectionGraph(const CellIntersectionGraph&) = delete;
    void operator=(const CellIntersectionGraph&) = delete;

    explicit CellIntersectionGraph(CellIntGraphFR* M);
    virtual ~CellIntersectionGraph();

    bool IsMonochromatic(Vertex, Vertex) const;
    bool IsMonochromatic(VertexPair) const;
    Multicolor CommonColors(Vertex, Vertex) const;
    Multicolor CommonColors(VertexPair) const;
    size_t CommonColorCount(Vertex, Vertex) const;
    size_t CommonColorCount(VertexPair) const;
    const Multicolor& vertex_color(Vertex v) const {
        return vertex_colors_[v];
    }
    Weight FillCount(Vertex u, Vertex v) const {
        return CommonColorCount(u, v);
    }
    Weight FillCount(VertexPair p) const {
        return FillCount(p.first, p.second);
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
};  // CellIntersectionGraph

}  // namespace chordalg

#endif  // INCLUDE_INTERSECTION_GRAPH_H_
