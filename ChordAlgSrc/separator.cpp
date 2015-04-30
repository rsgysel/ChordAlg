#include "ChordAlgSrc/separator.h"

#include <iostream>
#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors
SeparatorComponents::SeparatorComponents(const Graph* G) :
    G_(G),
    S_(G_->order()),
    connected_component_(),
    search_queue_(),
    size_(0) {
    S_.reserve(G_->order());
    connected_component_.resize(G_->order());
    search_queue_.reserve(G_->order());
    return;
}

SeparatorBlocks::SeparatorBlocks(const Graph* G) :
    SeparatorComponents(G),
    blocks_(),
    last_separator_vertex_seen_() {
    last_separator_vertex_seen_.resize(G_->order());
    return;
}

////////////// Methods

void SeparatorComponents::SeparateNbhd(Vertex v) {
    FillSet fill;
    InitializeS(G_->N(v));
    FindComponents(fill);
    return;
}

void SeparatorComponents::SeparateClosedNbhd(Vertex v) {
    Vertices S;
    S.add(v);
    for (Vertex u : G_->N(v)) {
        S.add(u);
    }
    FillSet fill;
    InitializeS(S);
    this->FindComponents(fill);
    return;
}

void SeparatorComponents::Separate(const Vertices& S) {
    FillSet fill;
    InitializeS(S);
    FindComponents(fill);
    return;
}

// Main computation
void SeparatorComponents::Separate(const Vertices& S, FillSet& fill) {
    InitializeS(S);
    FindComponents(fill);
    return;
}

void SeparatorComponents::InitializeS(const Vertices& S) {
    size_ = 0;
    // intialize separator
    S_.clear();
    for (Vertex v : S) {
        S_.add(v);
    }
    // initialize connected component of each vertex
    for (Vertex v : *G_) {
        connected_component_[v] = kUnsearched();
    }
    for (Vertex v : S) {
        connected_component_[v] = kInSeparator();
    }
    return;
}

// Finds connected components
void SeparatorComponents::FindComponents(FillSet& fill) {
    ConnectedComponentID current_component = kUnsearched();
    for (Vertex v : *G_) {
        if (IsUnsearched(v) && !IsInSeparator(v)) {
            search_queue_.push_back(v);
            ++current_component;
            connected_component_[v] = current_component;
        }  // if new connected component found
        while (!search_queue_.empty()) {
            Vertex u = search_queue_.back();
            search_queue_.pop_back();
            // BFS
            for (Vertex w : GetNeighborhood(u, fill)) {
                if (IsUnsearched(w) && !IsInSeparator(w)) {
                    search_queue_.push_back(w);
                    connected_component_[w] = current_component;
                }  // if w is unsearched and not in S
            }
        }
    }
    size_ = current_component + 1;
    return;
}

// Finds connected component containing v
Vertices SeparatorComponents::ConnectedComponent(Vertex v) const {
    Vertices C;
    if (IsInSeparator(v)) {
        return C;
    }
    for (Vertex u : *G_) {
        if (AreConnected(u, v)) {
            C.add(u);
        }
    }
    return C;
}

Vertices SeparatorComponents::GetNeighborhood(Vertex u, FillSet& fill) {
    if (fill.empty()) {
        return Vertices(G_->N(u));
    } else {
        Vertices neighborhood;
        for (Vertex v : G_->N(u)) {
            neighborhood.add(v);
        }
        for (Vertex v : fill[u]) {
            neighborhood.add(v);
        }
        return neighborhood;
    }
}

void SeparatorBlocks::FindComponents(FillSet& fill) {
    SeparatorComponents::FindComponents(fill);
    blocks_.clear();
    blocks_.resize(size_);
    for (Vertex v : *G_) {
        if (!IsInSeparator(v)) {
            ConnectedComponentID C = connected_component_[v];
            blocks_[C].addC(v);
        }
    }
    last_separator_vertex_seen_.clear();
    FindNeighborhoods(fill);
    return;
}

// Finds the neighborhoods of connected components.
// Our implementation is an extension of the algorithm found on p.50 in:
// paper: http://www.sciencedirect.com/science/article/pii/S0196677404001142
void SeparatorBlocks::FindNeighborhoods(FillSet& fill) {
    // used to ensure each element of N(C) appears once
    last_separator_vertex_seen_.resize(size_);
    for (ConnectedComponentID &C : last_separator_vertex_seen_) {
        C = -1;
    }
    S_.sort();
    for (Vertex v : S_) {
        const Vertices& neighborhood = GetNeighborhood(v, fill);
        for (Vertex u : neighborhood) {
            ConnectedComponentID C = connected_component_[u];
            // if u is not a separator vertex, u is in C, and we haven't
            // determined that v is in N(C)
            if (!IsInSeparator(u) && 
                last_separator_vertex_seen_[C] != static_cast<int>(v)) {
                blocks_[C].addNC(v);
                last_separator_vertex_seen_[C] = v;
            }
        }
    }
    return;
}

size_t SeparatorBlocks::FullComponentCt() const {
    size_t count = 0;
    for (Block B : blocks_) {
        if (B.NC().size() == S_.size()) {
            ++count;
        } else if (B.NC().size() > S_.size()) {
            std::cerr << "Error in SeparatorBlocks::FullComponentCt: Full ";
            std::cerr << "component neighborhood larger than separator size.";
            std::cerr << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return count;
}

size_t SeparatorBlocks::NonFullComponentCt() const {
    return size_ - FullComponentCt();
}

bool SeparatorComponents::IsSeparated(Vertices V) const {
    for (auto p : VertexPairs(V)) {
        if (AreSeparated(p.first, p.second)) {
            return true;
        }
    }
    return false;
}

std::string SeparatorComponents::str() const {
    std::string SCstr;
    for (Vertex v : *G_) {
        SCstr += "CC(" + G_->name(v) + "): "
                 + std::to_string(connected_component_[v]) + '\n';
    }
    SCstr += '\n';
    return SCstr;
}

std::string SeparatorBlocks::str() const {
    std::string SCstr = SeparatorComponents::str();
    ConnectedComponentID cc = 0;
    for (Block B : blocks_) {
        SCstr += "N(C_" + std::to_string(cc) + "): " + G_->str(B.NC());
        ++cc;
    }
    return SCstr;
}

}  // namespace chordalg

