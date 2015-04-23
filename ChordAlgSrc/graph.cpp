#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "graph.h"

namespace chordalg {

Graph::Graph(Graph& H) :
    neighborhoods_(new AdjacencyLists(*(H.neighborhoods_))),
    vertex_names_(new VertexNames(*(H.vertex_names_))),
    order_(H.order_),
    size_(H.size_) {
    Init();
    return;
}

Graph::Graph(FileReader* fr) :
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

InducedSubgraph::InducedSubgraph(const Graph* G, Vertices U) :
    Graph(InducedVertices(*G, U)),
    G_(G),
    U_(U) {
    return;
}

Graph::~Graph() {
    delete neighborhoods_;
    delete vertex_names_;
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
    // edge consistency
    for (Vertex x : *this) {
        for (Vertex y : *this) {
            if (is_edge_[x][y] != is_edge_[y][x]) {
                std::cerr << "Error: edge relationships not symmetric wrt ";
                std::cerr << name(x) << " and " << name(y) << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return;
}

AdjacencyLists* Graph::InducedVertices(const Graph& super_graph, Vertices X) {
    std::sort(X.begin(), X.end());
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(X.size());
    std::map< Vertex, int > vertex_order;
    int i = 0;
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
    for (Vertex v : X) {
        for (Vertex u : super_graph.N(v)) {
            if (in_subgraph[u]) {
                Vertex v_induced_id = vertex_order[v];
                Vertex u_induced_id = vertex_order[u];
                (*a_lists)[v_induced_id].add(u_induced_id);
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

// Determines if H is isomorphic to G with respect to their fixed orderings
bool Graph::IsIsomorphic(Graph& H) const {
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

std::string Graph::str() const {
    std::string Gstr = std::to_string(order_) + '\n';
    for (Vertex v : *this) {
        Gstr += name(v) + ' ';
        for (Vertex u : N(v)) {
            Gstr += name(u) + ' ';
        }
        Gstr.erase(Gstr.end() - 1, Gstr.end());
        Gstr += '\n';
    }
    return Gstr;
}

std::string Graph::str(const LexTrie& T) const {
    std::string Tstr;
    for (FiniteSet S : T) {
        Tstr += str(Vertices(S));
    }
    return Tstr;
}

std::string Graph::str(const VertexVector& U) const {
    return str(Vertices(U));
}

std::string Graph::str(const Vertices& U) const {
    if (U.empty()) {
        return std::string();
    } else {
        std::string Ustr;
        for (Vertex v : U) {
            Ustr += name(v) + ' ';
        }
        Ustr.erase(Ustr.end() - 1, Ustr.end());
        Ustr += '\n';
        return Ustr;
    }
}

}  // namespace chordalg
