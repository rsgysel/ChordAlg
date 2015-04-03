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

ColoredIntersectionGraph::ColoredIntersectionGraph(
    ColoredIntersectionGraph& super_graph, Vertices X) :
    Graph(super_graph, X),
    subsets_(InduceSubsets(super_graph, X)),
    vertex_colors_(InduceVertexColors(super_graph, X)),
    subset_family_(InduceSubsetFamily(super_graph)),
    taxon_name_(DefaultTaxonNames(subset_family_->n())) {
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

std::vector< Subset > ColoredIntersectionGraph::InduceSubsets(
    ColoredIntersectionGraph& super_graph, Vertices X) {
    std::sort(X.begin(), X.end());
    std::vector< Subset > subsets;
    subsets.reserve(X.size());
    for (Vertex v : X) {
        subsets.push_back(super_graph.subsets_[v]);
    }
    return subsets;
}

std::vector< Multicolor > ColoredIntersectionGraph::InduceVertexColors(
    ColoredIntersectionGraph& super_graph, Vertices X) {
    std::sort(X.begin(), X.end());
    std::vector< Multicolor > vertex_colors;
    vertex_colors.reserve(X.size());
    for (Vertex v : X) {
        vertex_colors.push_back(super_graph.vertex_colors_[v]);
    }
    return vertex_colors;
}

LexTrie* ColoredIntersectionGraph::InduceSubsetFamily(
    ColoredIntersectionGraph& super_graph) {
    LexTrie* subset_family = new LexTrie(super_graph.subset_family_->n());
    for (Subset& S : subsets_) {
        subset_family->SortedInsert< Subset >(S);
    }
    return subset_family;
}

bool ColoredIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) {
    return CommonColorCount(u, v) > 0;
}

size_t ColoredIntersectionGraph::CommonColorCount(Vertex u, Vertex v) {
    Multicolor intersection;
    std::set_intersection(vertex_color(u).begin(), vertex_color(u).end(),
                          vertex_color(v).begin(), vertex_color(v).end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection.size();
}

void ColoredIntersectionGraph::PrettyPrintSubsets() {
    for (Vertex v : *this) {
        std::cout << "Subset(" << name(v) << "): ";
        std::copy(subsets_[v].begin(), subsets_[v].end(),
                  std::ostream_iterator< Vertex >(std::cout, " "));
        std::cout << std::endl;
        std::cout << "Colors: ";
        std::copy(vertex_colors_[v].begin(), vertex_colors_[v].end(),
                  std::ostream_iterator< Color >(std::cout, " "));
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}

}  // namespace chordalg