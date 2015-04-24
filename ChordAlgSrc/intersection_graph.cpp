#include "intersection_graph.h"

#include <algorithm>
#include <string>
#include <vector>

namespace chordalg {

//////////////////
// ctor's & dtor's

CharacterIntersectionGraph::CharacterIntersectionGraph(CharacterIntGraphFR* fr) :
    Graph(fr),
    subsets_(fr->subsets()),
    taxon_name_(fr->taxon_name()) {
    return;
}

CharacterIntersectionGraph::~CharacterIntersectionGraph() {
    return;
}

PartitionIntersectionGraph::PartitionIntersectionGraph(PartitionIntGraphFR* fr) :
    CharacterIntersectionGraph(fr),
    vertex_color_(fr->vertex_color()) {
    return;
}

PartitionIntersectionGraph::~PartitionIntersectionGraph() {
    return;
}

CellIntersectionGraph::CellIntersectionGraph(CellIntGraphFR* fr) :
    CharacterIntersectionGraph(fr),
    vertex_colors_(fr->vertex_colors()),
    subset_family_(fr->TakeSubsetFamily()) {
    return;
}

CellIntersectionGraph::~CellIntersectionGraph() {
    delete subset_family_;
    return;
}

/////////////////
// Common methods

Weight CharacterIntersectionGraph::FillCount(VertexPair uv) const {
    return FillCount(uv.first, uv.second);
}

Weight PartitionIntersectionGraph::FillCount(Vertex u, Vertex v) const {
    return IsMonochromatic(u, v) ? 1 : 0;
}

Weight CellIntersectionGraph::FillCount(Vertex u, Vertex v) const {
    return CommonColorCount(u, v);
}

bool CharacterIntersectionGraph::IsMonochromatic(VertexPair uv) const {
    return IsMonochromatic(uv.first, uv.second);
}

bool PartitionIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return vertex_color_[u] == vertex_color_[v];
}

bool CellIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return CommonColorCount(u, v) > 0;
}

Color PartitionIntersectionGraph::vertex_color(Vertex v) const {
    return vertex_color_[v];
}

const Multicolor& CellIntersectionGraph::vertex_color(Vertex v) const {
    return vertex_colors_[v];
}

//////////
// Methods

const FiniteSet& CharacterIntersectionGraph::subset(Vertex v) const {
    return subsets_[v];
}

const std::string CharacterIntersectionGraph::taxon_name(size_t t) const {
    return taxon_name_[t];
}

size_t CharacterIntersectionGraph::taxa() const {
    return taxon_name_.size();
}

std::string CharacterIntersectionGraph::strSubsets() {
    std::string Sstr;
    for (Vertex v : *this) {
        Sstr += "Taxa(" + name(v) + "): ";
        std::ostringstream oss;
        std::copy(subsets_[v].begin(), subsets_[v].end() - 1,
                  std::ostream_iterator< Vertex >(oss, " "));
        oss << subsets_[v].back();
        Sstr += oss.str() + '\n';
    }
    Sstr += '\n';
    return Sstr;
}

Multicolor CellIntersectionGraph::CommonColors(Vertex u, Vertex v) const {
    Multicolor intersection;
    std::set_intersection(vertex_color(u).begin(), vertex_color(u).end(),
                          vertex_color(v).begin(), vertex_color(v).end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}

Multicolor CellIntersectionGraph::CommonColors(VertexPair uv) const {
    return CommonColors(uv.first, uv.second);
}

size_t CellIntersectionGraph::CommonColorCount(Vertex u, Vertex v) const {
    return CommonColors(u, v).size();
}

size_t CellIntersectionGraph::CommonColorCount(VertexPair uv) const {
    return CommonColorCount(uv.first, uv.second);
}

const LexTrie* CellIntersectionGraph::subset_family() const {
    return subset_family_;
}

}  // namespace chordalg
