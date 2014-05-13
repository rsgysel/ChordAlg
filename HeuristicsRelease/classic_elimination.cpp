#include "classic_elimination.h"

namespace chordalg {

ClassicElimination::ClassicElimination( ColoredIntersectionGraph& H, ClassicCriterion* f ) :
    EliminationOrder( H ),
    H_              ( H ),
    f_              ( f )
{
    EliminationOrder::Init();
    return;
}

ClassicElimination::~ClassicElimination()
{
    return;
}

void ClassicElimination::Eliminate( Vertex v )
{
    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
        AddEdge( p );
    return;
}

std::pair< Weight, Cost > ClassicElimination::WeightOf( Vertex v )
{
    Weight wt = 0;
    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
    {
        if( !IsEdge( p ) )
            wt += H_.CommonColorCount( p.first, p.second );
    }
    return std::pair< Weight, Cost >( f_->Calculate( wt ), wt );
}

} // namespace chordalg
