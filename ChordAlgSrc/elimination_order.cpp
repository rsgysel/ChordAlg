#include "ChordAlgSrc/elimination_order.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "ChordAlgSrc/chordalg_string.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

EliminationOrder::EliminationOrder(const Graph* G) :
    G_(G),
    alpha_(G_->order()),
    alpha_inverse_(G_->order()) {
    Init();
    return;
}

EliminationOrder::~EliminationOrder() {
    return;
}

///////////////////
// EliminationOrder

EliminationOrder* EliminationOrder::File(
    const Graph* G,
    std::string filename) {
    EliminationOrder* eo = new EliminationOrder(G);
    std::fstream file_stream;
    file_stream.open(filename.c_str());
    if (!file_stream) {
        std::cerr << "Error: can't open " << filename << '\n';
        exit(EXIT_FAILURE);
    }
    std::string file_line;
    std::getline(file_stream, file_line);
    file_stream.close();
    StringTokens tokens = Split(file_line, " \t");
    std::map< std::string, Vertex > vertex_id;
    for (Vertex v : *G) {
        vertex_id[G->name(v)] = v;
    }
    std::set< Vertex > vertices_emplaced;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (vertex_id.find(tokens[i]) == vertex_id.end()) {
            std::cerr << "Vertex " << tokens[i] << " not in graph\n";
            exit(EXIT_FAILURE);
        }
        Vertex v = vertex_id[tokens[i]];
        eo->Emplace(v, i);
        vertices_emplaced.insert(v);
    }
    // Check that elimination order file is a permutation
    if (vertices_emplaced.size() != G->order() ||
        tokens.size() != G->order()) {
        std::cerr << "File " << filename
                  << " is not an elimination ordering (permutation)\n";
        exit(EXIT_FAILURE);
    }
    return eo;
}

void EliminationOrder::Init() {
    int i = 0;
    for (Vertex v : *G_) {
        alpha_[i] = v;
        alpha_inverse_[v] = i;
        ++i;
    }
    return;
}

void EliminationOrder::Emplace(Vertex v, int i) {
    Swap(i, alpha_inverse_[v]);
    return;
}

void EliminationOrder::Swap(int i, int j) {
    Vertex  vi = alpha_[i],
            vj = alpha_[j];
    std::swap(alpha_[i], alpha_[j]);
    std::swap(alpha_inverse_[vi], alpha_inverse_[vj]);
    return;
}

bool EliminationOrder::Before(Vertex u, Vertex v) const {
    return alpha_inverse_[u] < alpha_inverse_[v];
}
int EliminationOrder::PositionOf(Vertex v) const {
    return alpha_inverse_[v];
}
Vertex EliminationOrder::VertexAt(int i) const {
    return alpha_[i];
}

Vertices EliminationOrder::LNbhd(Vertex v, const FillEdges* F) const {
    Vertices L_N;
    for (Vertex u : G_->N(v)) {
        if (Before(u, v)) {
            L_N.push_back(u);
        }
    }
    if (F) {
        for (Vertex u : (*F)[v]) {
            if (Before(u, v)) {
                L_N.push_back(u);
            }
        }
    }
    return L_N;
}

Vertices EliminationOrder::RNbhd(Vertex v, const FillEdges* F) const {
    Vertices R_N;
    for (Vertex u : G_->N(v)) {
        if (Before(v, u)) {
            R_N.push_back(u);
        }
    }
    if (F) {
        for (Vertex u : (*F)[v]) {
            if (Before(v, u)) {
                R_N.push_back(u);
            }
        }
    }
    return R_N;
}

size_t EliminationOrder::size() const {
    return alpha_.size();
}
const Graph* EliminationOrder::G() const {
    return G_;
}

void EliminationOrder::SetOrder(const VertexVector& pi) {
    if (pi.size() != G_->order()) {
        std::cerr << "Error in SetOrder: elimination order size and graph ";
        std::cerr << "order does not match" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < pi.size(); ++i) {
        Emplace(pi[i], i);
    }
    return;
}

std::string EliminationOrder::str() const {
    std::string Estr;
    for (Vertex v : alpha_) {
        Estr += G_->name(v) + " ";
    }
    Estr.pop_back();
    Estr += '\n';
    return Estr;
}

}  // namespace chordalg
