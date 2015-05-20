/*
 *  intersection_graph.h - an intersection graph data structure
 */

#ifndef CHORDALGSRC_INTERSECTION_GRAPH_H_
#define CHORDALGSRC_INTERSECTION_GRAPH_H_

#include <string>
#include <vector>

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class CharacterIntersectionGraph : public Graph {
 public:
    CharacterIntersectionGraph() = delete;
    CharacterIntersectionGraph(const CharacterIntersectionGraph&) = delete;
    void operator=(const CharacterIntersectionGraph&) = delete;

    explicit CharacterIntersectionGraph(CharacterIntGraphFR*);
    virtual ~CharacterIntersectionGraph();

    virtual Weight FillCount(Vertex, Vertex) const = 0;
    Weight FillCount(VertexPair) const;
    virtual bool IsMonochromatic(Vertex, Vertex) const = 0;
    bool IsMonochromatic(VertexPair) const;

    const FiniteSet& subset(Vertex) const;
    const std::string& taxon_name(size_t) const;
    const Vertices& taxon_clique(Vertex) const;
    size_t taxa() const;
    std::string strSubsets();

 protected:
    const std::vector< FiniteSet >* subsets_;
    const std::vector< std::string >* taxon_name_;
    const std::vector< Vertices >* taxon_clique_;
};  // CharacterIntersectionGraph

class PartitionIntersectionGraph : public CharacterIntersectionGraph {
 public:
    PartitionIntersectionGraph() = delete;
    PartitionIntersectionGraph(const PartitionIntersectionGraph&) = delete;
    void operator=(const PartitionIntersectionGraph&) = delete;

    explicit PartitionIntersectionGraph(PartitionIntGraphFR*);
    virtual ~PartitionIntersectionGraph();

    static PartitionIntersectionGraph* New(std::string);
    static PartitionIntersectionGraph* New(GraphFile*);

    using CharacterIntersectionGraph::FillCount;
    Weight FillCount(Vertex, Vertex) const;
    bool IsMonochromatic(Vertex, Vertex) const;
    Color vertex_color(Vertex) const;

 protected:
    const std::vector< Color >* vertex_color_;
};  // PartitionIntersectionGraph

class CellIntersectionGraph : public CharacterIntersectionGraph {
 public:
    CellIntersectionGraph() = delete;
    CellIntersectionGraph(const CellIntersectionGraph&) = delete;
    void operator=(const CellIntersectionGraph&) = delete;

    static CellIntersectionGraph* New(std::string);
    static CellIntersectionGraph* New(GraphFile*);

    explicit CellIntersectionGraph(CellIntGraphFR*);
    virtual ~CellIntersectionGraph();

    using CharacterIntersectionGraph::FillCount;
    Weight FillCount(Vertex, Vertex) const;
    bool IsMonochromatic(Vertex, Vertex) const;
    const Multicolor& vertex_color(Vertex) const;

    Multicolor CommonColors(Vertex, Vertex) const;
    Multicolor CommonColors(VertexPair) const;
    size_t CommonColorCount(Vertex, Vertex) const;
    size_t CommonColorCount(VertexPair) const;

 protected:
    const std::vector< Multicolor >* vertex_colors_;
};  // CellIntersectionGraph

}  // namespace chordalg

#endif  // CHORDALGSRC_INTERSECTION_GRAPH_H_
