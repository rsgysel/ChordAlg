#include "graph.h"

namespace chordalg{

////////////// ctor & dtors
//
Graph::Graph(FileReader* fr) : neighborhoods_(fr->TakeNeighborhoods()),
    vertex_names_(fr->TakeNames()),
    order_(neighborhoods_->size()),
    size_(0),
    graph_id_(reinterpret_cast<int>(this))
{
    for(Nbhd const& N : *neighborhoods_)
        size_ += N.size();
    size_ /= 2;

    return;
}

Graph::~Graph(){
    delete neighborhoods_;
    delete vertex_names_;
}

}   // namespace chordalg
