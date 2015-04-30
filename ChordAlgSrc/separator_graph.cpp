#include "ChordAlgSrc/separator_graph.h"

#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

MinsepVector* MinsepTrieToVector(const MinsepTrie& MT) {
    MinsepVector* MV = new MinsepVector();
    MV->resize(MT.size());
    size_t i = 0;
    for (auto S : MT) {
        (*MV)[i] = Vertices(VertexVector(S));
        ++i;
    }
    return MV;
}

SeparatorGraph::SeparatorGraph(const Graph* G, const MinsepVector* M) :
    Graph(M->size()),
    G_(G),
    M_(M),
    crossing_relations_(),
    S_(G_) {
    Init();
}

SeparatorGraph::SeparatorGraph(const Graph* G, const MinsepTrie* M) :
    Graph(M->size()),
    G_(G),
    M_(MinsepTrieToVector(*M)),
    crossing_relations_(),
    S_(G_) {
    delete M;
    Init();
}

SeparatorGraph::~SeparatorGraph() {
    delete M_;
}

void SeparatorGraph::Init() {
    size_t i = 0;
    crossing_relations_.resize(M_->size());
    // Compute crossing relations
    for (auto U : *M_) {
        S_.Separate(U);
        size_t j = 0;
        for (auto W : *M_) {
            if (i < j && S_.IsSeparated(W)) {
                crossing_relations_[i].add(j);
                crossing_relations_[j].add(i);
                ++size_;
            }
            ++j;
        }
        ++i;
    }
    return;
}

std::string SeparatorGraph::str() const {
    std::string SGstr = "Order" + std::to_string(order_) + '\n'
                        + "Size" + std::to_string(size_) + '\n';
    for (Vertex v : *this) {
        SGstr += 'S' + std::to_string(v) + ':' + name(v) + '\n';
    }
    SGstr += '\n';
    for (Vertex v : *this) {
        SGstr += "N(S" + std::to_string(v) + "): ";
        for (Vertex u : N(v)) {
            SGstr += 'S' + std::to_string(u) + ' ';
        }
        SGstr += '\n';
    }
    return SGstr;
}

VertexName SeparatorGraph::name(Vertex v) const {
    const Vertices& S_v = (*M_)[v];
    if (S_v.empty()) {
        return std::string();
    } else {
        std::string name;
        for (auto itr = S_v.begin(); itr != S_v.end() - 1; ++itr) {
            name += G_->name(*itr) + ' ';
        }
        name += G_->name(*(S_v.end() - 1));
        return name;
    }
}

// uv is an edge iff S_u crosses (separates) S_v
bool SeparatorGraph::HasEdge(Vertex u, Vertex v) const {
    SeparatorComponents S(G_);
    S.Separate((*M_)[u]);
    return S.IsSeparated((*M_)[v]);
}

}  // namespace chordalg
