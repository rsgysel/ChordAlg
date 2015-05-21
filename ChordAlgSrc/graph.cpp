#include "ChordAlgSrc/graph.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Graph::Graph(const Graph& G) :
    neighborhoods_(new AdjacencyLists(*G.neighborhoods_)),
    vertex_names_(new VertexNames(*G.vertex_names_)),
    order_(G.order()),
    size_(G.size()) {
    Init();
    return;
}

Graph::Graph(GraphFR* fr) :
    neighborhoods_(fr->TakeNeighborhoods()),
    vertex_names_(fr->TakeNames()),
    order_(neighborhoods_->size()),
    size_(0) {
    Init();
    return;
}

Graph::Graph(AdjacencyLists* a_lists) :
    neighborhoods_(a_lists),
    vertex_names_(DefaultNames(a_lists->size())),
    order_(a_lists->size()),
    size_(0) {
    Init();
    return;
}

Graph::Graph(AdjacencyLists* a_lists, VertexNames vertex_names) :
    neighborhoods_(a_lists),
    vertex_names_(new VertexNames(vertex_names)),
    order_(a_lists->size()),
    size_(0) {
    Init();
    return;
}

// Use this constructor in a subclass to defer initialization, adjacency lists,
// and vertex names to the subclass
Graph::Graph(size_t n) :
    neighborhoods_(nullptr),
    vertex_names_(nullptr),
    order_(n),
    size_(0) {
    return;
}

Graph::~Graph() {
    delete neighborhoods_;
    delete vertex_names_;
}

InducedSubgraph::InducedSubgraph(const Graph* G, const Vertices* U) :
    Graph(InducedVertices(*G, *U)),
    G_(G),
    U_(*U) {
    return;
}

////////
// Graph

Graph* Graph::New(std::string filename) {
    GraphFile* file = GraphFile::New(filename);
    Graph* G = New(file);
    delete file;
    return G;
}

Graph* Graph::New(GraphFile* file) {
    Graph* G = nullptr;
    if (file->file_type() == FileType::ADJLIST ||
        file->file_type() == FileType::DIMACS) {
        GraphFR* file_reader = GraphFR::New(file);
        G = new Graph(file_reader);
        delete file_reader;
    } else if (file->file_type() == FileType::CHARACTERMATRIX ||
               file->file_type() == FileType::NEXUSMRP) {
        CellIntGraphFR* file_reader = CellIntGraphFR::New(file);
        G = new Graph(file_reader);
        delete file_reader;
    }
    return G;
}

// Determines if H is isomorphic to G with respect to their
// vertex orderings
bool Graph::IsIsomorphic(const Graph& H) const {
    const Graph& G = *this;
    if (G.order() != H.order() || G.size() != H.size()) {
        return false;
    }
    for (Vertex v : G) {
        if (G.N(v).size() != H.N(v).size()) {
            return false;
        }
        if (!std::equal(G.N(v).begin(), G.N(v).end(), H.N(v).begin())) {
            return false;
        }
    }
    return true;
}

bool Graph::IsSupergraph(const Graph& H) const {
    const Graph& G = *this;
    if (G.order() != H.order()) {
        return false;
    }
    std::set< VertexName > names;
    typedef std::pair< VertexName, VertexName > EdgeName;
    std::set< EdgeName > edges;
    for (Vertex v : G) {
        names.insert(G.name(v));
        for (Vertex u : G.N(v)) {
            std::string u_str = G.name(u), v_str = G.name(v);
            if (u_str < v_str) {
                edges.insert(EdgeName(u_str, v_str));
            }
        }
    }
    for (Vertex v : H) {
        if (names.find(H.name(v)) == names.end()) {
            return false;
        }
        for (Vertex u : H.N(v)) {
            std::string u_str = H.name(u), v_str = H.name(v);
            if (u_str < v_str) {
                if (edges.find(EdgeName(u_str, v_str)) == edges.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::string Graph::str() const {
    std::string result = std::to_string(order_) + '\n';
    for (Vertex v : *this) {
        result += name(v) + ' ';
        for (Vertex u : N(v)) {
            result += name(u) + ' ';
        }
        result.pop_back();
        result += '\n';
    }
    return result;
}

std::string Graph::str(const LexTrie& T) const {
    std::string result;
    for (FiniteSet S : T) {
        Vertices V(S);
        result += str(V);
        result += '\n';
    }
    result.pop_back();
    return result;
}

std::string Graph::str(const VertexVector& U) const {
    Vertices V(U);
    return str(V);
}

std::string Graph::str(const Vertices& U) const {
    if (U.empty()) {
        return std::string();
    } else {
        std::string result;
        for (Vertex v : U) {
            result += name(v) + ' ';
        }
        result.pop_back();
        return result;
    }
}

// Outputs graph in DOT format
// http://en.wikipedia.org/wiki/DOT_%28graph_description_language%29
std::string Graph::strDOT(const std::string& graphname) const {
    std::string result("graph ");
    result += graphname + std::string(" {\n");
    for (Vertex v : *this) {
        result += std::string("\t") + name(v);
        result += std::string(" [shape=box];\n");
    }
    for (Vertex v : *this) {
        for (Vertex u : N(v)) {
            if (u < v) {
                result += std::string("\t") + name(u);
                result += std::string(" -- ") + name(v);
                result += std::string(";\n");
            }
        }
    }
    result += "}\n";
    return result;
}

// Outputs graph in GML format
//
std::string Graph::strGML(const std::string& graphname) const {
    std::string result("graph [\n\tundirected 1\n");
    result += std::string("\tlabel \"") + graphname + std::string("\"\n");
    for (Vertex v : *this) {
        result += std::string("\tnode [\n\t\tid ") + std::to_string(v);
        result += std::string("\n\t\tlabel \"") + name(v);
        result += std::string("\"\n\t]\n");
    }
    for (Vertex v : *this) {
        for (Vertex u : N(v)) {
            if (u < v) {
                result += std::string("\tedge [\n\t\tsource ");
                result += std::to_string(u) + std::string("\n\t\ttarget ");
                result += std::to_string(v) + std::string("\n\t]\n");
            }
        }
    }
    result += "]\n";
    return result;
}

Vertices Graph::V() const {
    Vertices V(order_);
    for (size_t i = 0; i < V.size(); ++i) {
        V[i] = i;
    }
    return V;
}

GraphVertexIterator Graph::begin() const {
    return GraphVertices(this, order_).begin();
}

GraphVertexIterator Graph::end() const {
    return GraphVertices(this, order_).end();
}

const AdjacencyLists& Graph::neighborhoods() const {
    return *neighborhoods_;
}

VertexName Graph::name(Vertex v) const {
    return (*vertex_names_)[v];
}

size_t Graph::order() const {
    return order_;
}

size_t Graph::size() const {
    return size_;
}

bool Graph::IsEdge(Vertex u, Vertex v) const {
    return is_edge_[u][v];
}

bool Graph::IsEdge(VertexPair uv) const {
    return IsEdge(uv.first, uv.second);
}

Weight Graph::FillCount(Vertex u, Vertex v) const {
    return IsEdge(u, v) ? 0 : 1;
}

Weight Graph::FillCount(VertexPair uv) const {
    return FillCount(uv.first, uv.second);
}

bool Graph::IsClique() const {
    return 2 * size_ == order_ * (order_ - 1);
}

bool Graph::IsClique(const Vertices& K) const {
    for (VertexPair uv : VertexPairs(K)) {
        if (!IsEdge(uv)) {
            return false;
        }
    }
    return true;
}

const Vertices& Graph::N(Vertex v) const {
    return (*neighborhoods_)[v];
}

// Use to transform InducedSubgraph vertices to parent graph vertices
Vertex Graph::ParentGraph(Vertex v) const {
    return v;
}

VertexPair Graph::ParentGraph(VertexPair uv) const {
    return uv;
}

void Graph::Init() {
    for (Nbhd const& N : *neighborhoods_) {
        size_ += N.size();
    }
    size_ /= 2;
    // adjacency matrix
    is_edge_.resize(order_);
    for (std::vector< bool >& V : is_edge_) {
        V.resize(order_);
    }
    for (Vertex x : *this) {
        for (Vertex y : *this) {
            is_edge_[x][y] = false;
        }
    }
    for (Vertex x : *this) {
        for (Vertex y : this->N(x)) {
            is_edge_[x][y] = true;
        }
    }
    return;
}

AdjacencyLists* Graph::InducedVertices(
    const Graph& super_graph,
    const Vertices& U) {
    Vertices X(U);
    std::sort(X.begin(), X.end());
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(X.size());
    std::map< Vertex, size_t > vertex_order;
    size_t i = 0;
    for (Vertex v : X) {
        vertex_order[v] = i;
        ++i;
    }
    std::vector< bool > in_subgraph;
    in_subgraph.resize(super_graph.order());
    for (Vertex v : super_graph) {
        in_subgraph[v] = false;
    }
    for (Vertex v : X) {
        in_subgraph[v] = true;
    }
    // a_lists sorted as long as super_graph.N(v) is
    for (Vertex v : X) {
        for (Vertex u : super_graph.N(v)) {
            if (in_subgraph[u]) {
                Vertex v_induced_id = vertex_order[v];
                Vertex u_induced_id = vertex_order[u];
                (*a_lists)[v_induced_id].push_back(u_induced_id);
            }
        }
    }
    return a_lists;
}

VertexNames* Graph::DefaultNames(size_t order) {
    int n = order;
    VertexNames* names = new VertexNames();
    names->resize(n);
    for (int i = 0; i < n; ++i) {
        (*names)[i] = std::to_string(i);
    }
    return names;
}

//////////////////
// InducedSubgraph

VertexName InducedSubgraph::name(Vertex v) const {
    return G_->name(U_[v]);
}
Weight InducedSubgraph::FillCount(Vertex u, Vertex v) const {
    return G_->FillCount(U_[u], U_[v]);
}
Weight InducedSubgraph::FillCount(VertexPair uv) const {
    return G_->FillCount(U_[uv.first], U_[uv.second]);
}
Vertex InducedSubgraph::ParentGraph(Vertex v) const {
    return U_[v];
}
VertexPair InducedSubgraph::ParentGraph(VertexPair uv) const {
    return VertexPair(ParentGraph(uv.first), ParentGraph(uv.second));
}

}  // namespace chordalg
