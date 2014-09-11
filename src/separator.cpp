#include "separator.h"

namespace chordalg {

////////////// ctor & dtors
//
SeparatorComponents::SeparatorComponents(Graph const& G) :
    G_(G),
    S_(G.order()),
    connected_component_(),
    search_queue_(),
    size_(0) {
    int n = G.order();
    S_.reserve(n);
    connected_component_.resize(n);
    search_queue_.reserve(n);
    return;
}

SeparatorBlocks::SeparatorBlocks(Graph const& G) :
    SeparatorComponents(G),
    blocks_(),
    last_separator_vertex_seen_() {
    int n = G.order();
    last_separator_vertex_seen_.resize(n);
    return;
}

////////////// Methods

void SeparatorComponents::SeparateNbhd(Vertex v) {
    FillSet fill;
    InitializeS(G_.N(v));
    FindComponents(fill);
    return;
}

void SeparatorComponents::SeparateClosedNbhd(Vertex v) {
    Vertices S;
    S.add(v);
    for (Vertex u : G_.N(v)) {
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
    for (Vertex v : G_) {
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
    for (Vertex v : G_) {
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
    for (Vertex u : G_) {
        if (AreConnected(u, v)) {
            C.add(u);
        }
    }
    return C;
}

Vertices SeparatorComponents::GetNeighborhood(Vertex u, FillSet& fill) {
    if (fill.empty()) {
        return Vertices(G_.N(u));
    } else {
        Vertices neighborhood;
        for (Vertex v : G_.N(u)) {
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
    for (Vertex v : G_) {
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
            if (!IsInSeparator(u) && last_separator_vertex_seen_[C] != v) {
                blocks_[C].addNC(v);
                last_separator_vertex_seen_[C] = v;
            }
        }
    }
    return;
}

int SeparatorBlocks::FullComponentCt() const {
    int count = 0;
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

int SeparatorBlocks::NonFullComponentCt() const {
    return size_ - FullComponentCt();
}

void SeparatorComponents::PrettyPrint() const {
    for (Vertex v : G_) {
        std::cout << "CC(" << G_.name(v) << "): " << connected_component_[v];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}

void SeparatorBlocks::PrettyPrint() const {
    SeparatorComponents::PrettyPrint();
    ConnectedComponentID cc = 0;
    for (Block B : blocks_) {
        std::cout << "N(C_" << cc << "): ";
        G_.PrettyPrint(B.NC());
        ++cc;
    }
    return;
}

}  // namespace chordalg

