#include "connectedcomponentcalculator.h"

namespace chordalg {

////////////// ctor & dtors
//
ConnectedComponentCalculator::ConnectedComponentCalculator(Graph const& G) :
    G_(G),
    S_(),
    connected_component_(),
    search_queue_(),
    size_(0)
{
    int n = G.order();
    S_.resize(n);
    connected_component_.resize(n);
    search_queue_.resize(n); search_queue_.clear();
    return;
}

ConnectedComponentNeighborhoodCalculator::ConnectedComponentNeighborhoodCalculator(Graph const& G, ConnectedComponentCalculator const& CC) :
    G_(G),
    CC_(CC),
    size_(CC.size())
{
    int n = G.order();
    neighborhood_.resize(n);
    for(ComputationBuffer &NC : neighborhood_)
        NC.resize(n);

    return;
}

////////////// Methods
// Main computation
void ConnectedComponentCalculator::Calculate(VertexContainer const & S){
    Init(S);
    FindComponents();
    return;
}

// Finds connected components
void ConnectedComponentCalculator::FindComponents(){
    ConnectedComponentID current_component = kUnsearched();
    for(Vertex v : G_){
        if(connected_component_[v] == kUnsearched() && connected_component_[v] != kInSeparator())
        {
            search_queue_.push_back(v);
            ++current_component;
            connected_component_[v] = current_component;
        } // if new connected component found

        while(!search_queue_.empty()){
            // remove vertex from queue and note its connected component
            Vertex u = search_queue_.back(); search_queue_.pop_back();

            // BFS on u's neighborhood
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

// main computation
void ConnectedComponentNeighborhoodCalculator::Calculate(){
    Init();
    FindNeighborhoods();
    return;
}

// finds neighborhoods
void ConnectedComponentNeighborhoodCalculator::FindNeighborhoods(){
    // to ensure each element of N(C) appears once
    ComputationBuffer last_separator_vertex_seen;
    last_separator_vertex_seen.resize(CC_.size());
    for(ConnectedComponentID &C : last_separator_vertex_seen)
        C = -1;

    // for each v in S, use connected components to compute N(C)
    for(Vertex v : CC_.S())
    {
        for(Vertex u : G_.N(v))
        {
            ConnectedComponentID C = CC_[u];
            if(C != CC_.kInSeparator() && last_separator_vertex_seen[C] != v )
            {
                neighborhood_[C].push_back(v);
                last_separator_vertex_seen[C] = v;
            } // if u is not a separator vertex, u is in C, and we haven't determined that v is in N(C)
        }
    }

    return;
}

////////////// initialization of buffers for main computation
//
void ConnectedComponentCalculator::Init(VertexContainer const & S){
    // intialize separator
    S_.clear();
    for(Vertex v : S)
        S_.push_back(v);

    // initialize connected component of each vertex
    for(Vertex v : G_)
        connected_component_[v] = kUnsearched();
    for(Vertex v : S)
        connected_component_[v] = kInSeparator();

    return;
}

void ConnectedComponentNeighborhoodCalculator::Init(){

    for(ComputationBuffer &NC : neighborhood_)
        NC.clear();

    return;
}

} // namespace chordalg

