#include "ChordAlgSrc/separator.h"

#include <iostream>
#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Block::Block() : C_(), NC_() {
    return;
}

Block::Block(const Vertices& C) : C_(C), NC_() {
    return;
}

Block::Block(const Vertices& C, const Vertices& NC) :
    C_(C),
    NC_(NC) {
    return;
}

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

SeparatorComponents::~SeparatorComponents() {
    return;
}

SeparatorBlocks::SeparatorBlocks(const Graph* G) :
    SeparatorComponents(G),
    blocks_(),
    last_separator_vertex_seen_() {
    last_separator_vertex_seen_.resize(G_->order());
    return;
}

SeparatorBlocks::~SeparatorBlocks() {
    return;
}

////////
// Block

const Vertices& Block::C() const {
    return C_;
}

const Vertices& Block::NC() const {
    return NC_;
}

void Block::addC(Vertex v) {
    C_.push_back(v);
}

void Block::addNC(Vertex v) {
    NC_.push_back(v);
}

//////////////////////
// SeparatorComponents

Vertices SeparatorComponents::GetNeighborhood(Vertex u, const FillEdges* fill) {
    if (fill && fill->empty()) {
        return Vertices(G_->N(u));
    } else {
        Vertices neighborhood;
        for (Vertex v : G_->N(u)) {
            neighborhood.push_back(v);
        }
        if (fill) {
            for (Vertex v : (*fill)[u]) {
                neighborhood.push_back(v);
            }
        }
        return neighborhood;
    }
}

void SeparatorComponents::SeparateNbhd(Vertex v) {
    InitializeS(G_->N(v));
    FindComponents();
    return;
}

void SeparatorComponents::SeparateClosedNbhd(Vertex v) {
    Vertices S;
    S.push_back(v);
    for (Vertex u : G_->N(v)) {
        S.push_back(u);
    }
    InitializeS(S);
    FindComponents();
    return;
}

// Main computation
void SeparatorComponents::Separate(const Vertices& S, const FillEdges* fill) {
    InitializeS(S);
    FindComponents(fill);
    return;
}

// Connected component containing v
Vertices SeparatorComponents::ConnectedComponent(Vertex v) const {
    Vertices C;
    if (IsInSeparator(v)) {
        return C;
    }
    for (Vertex u : *G_) {
        if (AreConnected(u, v)) {
            C.push_back(u);
        }
    }
    return C;
}

size_t SeparatorComponents::size() const {
    return size_;
}

ConnectedComponentID SeparatorComponents::ComponentId(Vertex v) const {
    return connected_component_[v];
}

ConnectedComponentID SeparatorComponents::kInSeparator() const {
    return -2;
}
ConnectedComponentID SeparatorComponents::kUnsearched() const {
    return -1;
}
ConnectedComponentID SeparatorComponents::kRemoved() const {
    return -3;
}

inline bool SeparatorComponents::IsInSeparator(Vertex u) const {
    return connected_component_[u] == kInSeparator();
}

inline bool SeparatorComponents::AreConnected(Vertex u, Vertex v) const {
    return IsInSeparator(u) || IsInSeparator(v) ? false :
           connected_component_[u] == connected_component_[v];
}

// AreSeparated is not the complement of AreConnected, because vertices in
// S_ are neither connected or separated
inline bool SeparatorComponents::AreSeparated(Vertex u, Vertex v) const {
    return IsInSeparator(u) || IsInSeparator(v) ? false :
           connected_component_[u] != connected_component_[v];
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

void SeparatorComponents::InitializeS(const Vertices& S) {
    size_ = 0;
    // intialize separator
    S_.clear();
    for (Vertex v : S) {
        S_.push_back(v);
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
void SeparatorComponents::FindComponents(const FillEdges* fill) {
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

bool SeparatorComponents::IsUnsearched(Vertex u) const {
    return connected_component_[u] == kUnsearched();
}

//////////////////
// SeparatorBlocks

std::vector< Block >::const_iterator SeparatorBlocks::begin() {
    return blocks_.begin();
}

std::vector< Block >::const_iterator SeparatorBlocks::end() {
    return blocks_.end();
}

bool SeparatorBlocks::IsFull(ConnectedComponentID C) const {
    return S_.size() == NComponent(C).size();
}

const Vertices& SeparatorBlocks::Component(ConnectedComponentID C) const {
    return blocks_[C].C();
}

const Vertices& SeparatorBlocks::NComponent(ConnectedComponentID C) const {
    return blocks_[C].NC();
}

const Block& SeparatorBlocks::BlockOf(Vertex v) const {
    return blocks_[ComponentId(v)];
}

const Vertices& SeparatorBlocks::ComponentOf(Vertex v) const {
    return blocks_[ComponentId(v)].C();
}

const Vertices& SeparatorBlocks::NComponentOf(Vertex v) const {
    return blocks_[ComponentId(v)].NC();
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

std::string SeparatorBlocks::str() const {
    std::string SCstr = SeparatorComponents::str();
    ConnectedComponentID cc = 0;
    for (Block B : blocks_) {
        SCstr += "N(C_" + std::to_string(cc) + "): " + G_->str(B.NC());
        ++cc;
    }
    return SCstr;
}

void SeparatorBlocks::FindComponents(const FillEdges* fill) {
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
void SeparatorBlocks::FindNeighborhoods(const FillEdges* fill) {
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

}  // namespace chordalg

