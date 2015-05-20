#include "ChordAlgSrc/intersection_graph.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/graph_file.h"

namespace chordalg {

//////////////////
// ctor's & dtor's

CharacterIntersectionGraph::CharacterIntersectionGraph(
    CharacterIntGraphFR* fr) :
    Graph(fr),
    subsets_(fr->TakeSubsets()),
    taxon_name_(fr->TakeTaxonName()),
    taxon_clique_(fr->TakeTaxonClique()) {
    return;
}

CharacterIntersectionGraph::~CharacterIntersectionGraph() {
    return;
}

PartitionIntersectionGraph::PartitionIntersectionGraph(
    PartitionIntGraphFR* fr) :
    CharacterIntersectionGraph(fr),
    vertex_color_(fr->TakeVertexColor()) {
    return;
}

PartitionIntersectionGraph::~PartitionIntersectionGraph() {
    return;
}

CellIntersectionGraph::CellIntersectionGraph(CellIntGraphFR* fr) :
    CharacterIntersectionGraph(fr),
    vertex_colors_(fr->TakeVertexColors()) {
    return;
}

CellIntersectionGraph::~CellIntersectionGraph() {
    return;
}

/////////////////////////////
// CharacterIntersectionGraph

Weight CharacterIntersectionGraph::FillCount(VertexPair uv) const {
    return FillCount(uv.first, uv.second);
}

bool CharacterIntersectionGraph::IsMonochromatic(VertexPair uv) const {
    return IsMonochromatic(uv.first, uv.second);
}

const FiniteSet& CharacterIntersectionGraph::subset(Vertex v) const {
    return (*subsets_)[v];
}

const std::string& CharacterIntersectionGraph::taxon_name(size_t t) const {
    return (*taxon_name_)[t];
}

const Vertices& CharacterIntersectionGraph::taxon_clique(Vertex v) const {
    return (*taxon_clique_)[v];
}

size_t CharacterIntersectionGraph::taxa() const {
    return taxon_name_->size();
}

std::string CharacterIntersectionGraph::strSubsets() {
    std::string Sstr;
    for (Vertex v : *this) {
        Sstr += "Taxa(" + name(v) + "): ";
        std::ostringstream oss;
        std::copy((*subsets_)[v].begin(), (*subsets_)[v].end() - 1,
                  std::ostream_iterator< Vertex >(oss, " "));
        oss << (*subsets_)[v].back();
        Sstr += oss.str() + '\n';
    }
    Sstr += '\n';
    return Sstr;
}

/////////////////////////////
// PartitionIntersectionGraph

PartitionIntersectionGraph* PartitionIntersectionGraph::New(
    std::string filename) {
    GraphFile* file = GraphFile::New(filename);
    PartitionIntersectionGraph* G = New(file);
    delete file;
    return G;
}

PartitionIntersectionGraph* PartitionIntersectionGraph::New(GraphFile* file) {
    PartitionIntGraphFR* file_reader = PartitionIntGraphFR::New(file);
    PartitionIntersectionGraph* G = new PartitionIntersectionGraph(file_reader);
    delete file_reader;
    return G;
}

Weight PartitionIntersectionGraph::FillCount(Vertex u, Vertex v) const {
    return IsMonochromatic(u, v) ? 1 : 0;
}

bool PartitionIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return (*vertex_color_)[u] == (*vertex_color_)[v];
}

Color PartitionIntersectionGraph::vertex_color(Vertex v) const {
    return (*vertex_color_)[v];
}

////////////////////////
// CellIntersectionGraph

CellIntersectionGraph* CellIntersectionGraph::New(std::string filename) {
    GraphFile* file = GraphFile::New(filename);
    CellIntersectionGraph* G = New(file);
    delete file;
    return G;
}

CellIntersectionGraph* CellIntersectionGraph::New(GraphFile* file) {
    CellIntGraphFR* file_reader = CellIntGraphFR::New(file);
    CellIntersectionGraph* G = new CellIntersectionGraph(file_reader);
    delete file_reader;
    return G;
}

Weight CellIntersectionGraph::FillCount(Vertex u, Vertex v) const {
    return CommonColorCount(u, v);
}

bool CellIntersectionGraph::IsMonochromatic(Vertex u, Vertex v) const {
    return CommonColorCount(u, v) > 0;
}

const Multicolor& CellIntersectionGraph::vertex_color(Vertex v) const {
    return (*vertex_colors_)[v];
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

}  // namespace chordalg
