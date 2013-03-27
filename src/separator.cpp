#include "separator.h"

namespace chordalg {

////////////// ctor & dtors
//
SeparatorComponents::SeparatorComponents(Graph const& G) :
    G_(G),
    S_(),
    connected_component_(),
    search_queue_(),
    size_(0)
{
    int n = G.order();

    // allocate space
    S_.reserve(n);
    connected_component_.resize(n);

    search_queue_.reserve(n);
    return;
}

SeparatorBlocks::SeparatorBlocks(Graph const& G) :
    SeparatorComponents(G),
    neighborhoods_(),
    last_separator_vertex_seen_()
{
    int n = G.order();
    last_separator_vertex_seen_.resize(n);
    return;
}

////////////// Methods
// Main computation
void SeparatorComponents::Set(VertexContainer const & S){
    size_ = 0;

    // intialize separator
    S_.clear();
    for(Vertex v : S)
        S_.push_back(v);

    // initialize connected component of each vertex
    for(Vertex v : G_)
        connected_component_[v] = kUnsearched();
    for(Vertex v : S)
        connected_component_[v] = kInSeparator();

    // main calculation
    FindComponents();
    return;
}

// Finds connected component containing v
VertexContainer SeparatorComponents::ConnectedComponent(Vertex v) const {
    VertexContainer C;
    if( connected_component_[v] == kInSeparator() )
        return C;

    for( Vertex u : G_ )
    {
        if( connected_component_[u] == connected_component_[v] )
            C.push_back(u);
    }
    return C;
}

// Finds connected components
void SeparatorComponents::FindComponents(){
    ConnectedComponentID current_component = kUnsearched();
    for(Vertex v : G_){
        if(connected_component_[v] == kUnsearched() && connected_component_[v] != kInSeparator())
        {
            search_queue_.push_back(v);
            ++current_component;
            connected_component_[v] = current_component;
        } // if new connected component found

        while(!search_queue_.empty()){
            Vertex u = search_queue_.back(); search_queue_.pop_back();

            // BFS
            for(Vertex w : G_.N(u)){
                if(connected_component_[w] == kUnsearched() && connected_component_[w] != kInSeparator())
                {
                    search_queue_.push_back(w);
                    connected_component_[w] = current_component;
                } // if w is unsearched and not in S
            }
        }
    }

    size_ = current_component + 1;
    return;
}

void SeparatorBlocks::Set(VertexContainer const & S){
    SeparatorComponents::Set(S);

    neighborhoods_.resize(size_);
    last_separator_vertex_seen_.clear();

    // main computation
    FindNeighborhoods();
    return;
}

// Finds the neighborhoods of connected components.
// Our implementation is a trivial extension of the algorithm found on p.50 in the paper below.
// paper: http://www.sciencedirect.com/science/article/pii/S0196677404001142
// citation: http://www.informatik.uni-trier.de/~ley/db/journals/jal/jal58.html#BerryBHSV06
void SeparatorBlocks::FindNeighborhoods(){

    // used to ensure each element of N(C) appears once
    last_separator_vertex_seen_.resize(size_);
    for(ConnectedComponentID &C : last_separator_vertex_seen_)
        C = -1;

    // for each v in S, use connected components to compute N(C)
    for(Vertex v : S_)
    {
        for(Vertex u : G_.N(v))
        {
            ConnectedComponentID C = connected_component_[u];
            if(C != kInSeparator() && last_separator_vertex_seen_[C] != v )
            {
                neighborhoods_[C].push_back(v);
                last_separator_vertex_seen_[C] = v;
            } // if u is not a separator vertex, u is in C, and we haven't determined that v is in N(C)
        }
    }

    return;
}

} // namespace chordalg


