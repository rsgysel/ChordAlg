#include "mcs.h"

namespace chordalg {

EliminationOrder MCS(Graph& G)
{
    MCSQueue mcs_q( G.order() );
	EliminationOrder eo(G);

	for(int i = G.order() ; i > 0 ; i-- )
	{
	    Vertex v = mcs_q.Pop();
	    eo.Emplace(v, i-1);
		if( i != 1 )
		{
		    for( Vertex u : G.N(v) )
                mcs_q.Increment(u);
		}
	}

	return eo;
}
/*
EliminationOrder MCS(Graph& G)
{
    MCSQueue mcs_q( G.order() );
	EliminationOrder eo(G);

    int prev_card = 0;
	for(int i = G.order() ; i > 0 ; i-- )
	{
	    // Get max weight vertex and update others
	    int new_card = mcs_q.MaxWeight();
	    Vertex v = mcs_q.Pop();
	    eo.Emplace(v, i-1);
		if( i != 1 )
		{
		    for( Vertex u : G.N(v) )
            {
                mcs_q.Increment(u);
            }
		}
		// Check for maxclique construction
		if(new_card < prev_card)
        {
            //VertexSet K =
        }
	}

	return eo;
}*/

MCSQueue::MCSQueue(int order) :
    order_              ( order ),
    max_weight_         ( 0     ),
    remaining_vertices_ ( order )
{
    queue_.resize(order_ + 1);
    weight_.resize(order_);
    for(Vertex v = 0; v < order_; ++v)
    {
        queue_[0].insert(v);
        weight_[v] = 0;
    }
}

Vertex MCSQueue::Pop()
{
    if(remaining_vertices_ == 0)
    {
        std::cerr << "Error in MCSQueue: pop requested on empty queue" << std::endl;
        exit(EXIT_FAILURE);
    }
    Vertex v =  *( queue_[ max_weight_ ].begin() );
    queue_[ max_weight_ ].erase(v);
    weight_[v] = kDeletedVertex();
    --remaining_vertices_;
    while( remaining_vertices_ > 0 && queue_[ max_weight_ ].empty() )
    {
        --max_weight_;
    }
    return v;
}

void MCSQueue::Increment(Vertex v)
{
    if(weight_[v] != kDeletedVertex())
    {
        queue_[ weight_[v] ].erase(v);
        weight_[v]++;
        queue_[ weight_[v] ].insert(v);
        if(weight_[v] > max_weight_)
        {
            max_weight_++;
        }
    }
    return;
}

} // namespace chordalg
