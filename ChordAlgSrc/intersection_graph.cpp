#include "intersection_graph.h"

#include <algorithm>
#include <string>
#include <vector>

namespace chordalg {

ColoredIntersectionGraph::ColoredIntersectionGraph(MatrixCellIntGraphFR* fr) :
    Graph(fr),
    subsets_(fr->subsets()),
    vertex_colors_(fr->vertex_colors()),
    subset_family_(fr->TakeSubsetFamily()),
    taxon_name_(DefaultTaxonNames(subset_family_->n())) {
    return;
}

ColoredIntersectionGraph::ColoredIntersectionGraph(NexusMRPFR* fr) :
    Graph(fr),
    subsets_(fr->subsets()),
    vertex_colors_(fr->vertex_colors()),
    subset_family_(fr->TakeSubsetFamily()),
    taxon_name_(fr->TaxonName()) {
    return;
}

ColoredIntersectionGraph::~ColoredIntersectionGraph() {
    delete subset_family_;
    return;
}

std::vector<std::string> ColoredIntersectionGraph::DefaultTaxonNames(size_t n) {
    std::vector<std::string> taxon_name;
    taxon_name.resize(n);
    for (size_t i = 0; i < n; ++i) {
        std::stringstream ss;
        ss << i;
        taxon_name[i] = ss.str();
    }
    return taxon_name;
}

bool ColoredIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return CommonColorCount(u, v) > 0;
}

bool ColoredIntersectionGraph::IsMonochromatic(VertexPair uv) const {
    return IsMonochromatic(uv.first, uv.second);
}

Multicolor ColoredIntersectionGraph::CommonColors(Vertex u, Vertex v) const {
    Multicolor intersection;
    std::set_intersection(vertex_color(u).begin(), vertex_color(u).end(),
                          vertex_color(v).begin(), vertex_color(v).end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}

Multicolor ColoredIntersectionGraph::CommonColors(VertexPair uv) const {
    return CommonColors(uv.first, uv.second);
}

size_t ColoredIntersectionGraph::CommonColorCount(Vertex u, Vertex v) const {
    return CommonColors(u, v).size();
}

size_t ColoredIntersectionGraph::CommonColorCount(VertexPair uv) const {
    return CommonColorCount(uv.first, uv.second);
}

std::string ColoredIntersectionGraph::strSubsets() {
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
