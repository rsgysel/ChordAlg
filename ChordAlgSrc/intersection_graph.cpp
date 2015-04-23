#include "intersection_graph.h"

#include <algorithm>
#include <string>
#include <vector>

namespace chordalg {

CellIntersectionGraph::CellIntersectionGraph(CellIntGraphFR* fr) :
    Graph(fr),
    subsets_(fr->subsets()),
    vertex_colors_(fr->vertex_colors()),
    subset_family_(fr->TakeSubsetFamily()),
    taxon_name_(fr->taxon_name()) {
    return;
}

CellIntersectionGraph::~CellIntersectionGraph() {
    delete subset_family_;
    return;
}

bool CellIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return CommonColorCount(u, v) > 0;
}

bool CellIntersectionGraph::IsMonochromatic(VertexPair uv) const {
    return IsMonochromatic(uv.first, uv.second);
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

std::string CellIntersectionGraph::strSubsets() {
    std::string Sstr;
    for (Vertex v : *this) {
        Sstr += "Taxa(" + name(v) + "): ";
        std::ostringstream oss;
        std::copy(subsets_[v].begin(), subsets_[v].end() - 1,
                  std::ostream_iterator< Vertex >(oss, " "));
        oss << subsets_[v].back();
        Sstr += oss.str() + "\nColors: ";
        oss.str("");
        std::copy(vertex_colors_[v].begin(), vertex_colors_[v].end(),
                  std::ostream_iterator< Color >(oss, " "));
        Sstr += oss.str() + '\n';
    }
    Sstr += '\n';
    return Sstr;
}

}  // namespace chordalg
