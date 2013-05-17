#include "classic_heuristics.h"

namespace chordalg {

MonochromaticDeficiencyHeuristic::MonochromaticDeficiencyHeuristic(  ColoredIntersectionGraph& H ) :
    EliminationOrder( H ),
    H_ ( H )
{
    EliminationOrder::Init();
    return;
}

MonochromaticDeficiencyHeuristic::~MonochromaticDeficiencyHeuristic()
{
    return;
}

void MonochromaticDeficiencyHeuristic::Eliminate( Vertex v )
{
    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
        AddEdge( p );

    return;
}

std::pair< Weight, Cost > MonochromaticDeficiencyHeuristic::WeightOf( Vertex v )
{
    Weight wt = 0;

    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
    {
        if( !IsEdge( p ) )
            wt += H_.CommonColorCount( p.first, p.second );
    }

    return std::pair< Weight, Cost >( wt, wt );
}

} // namespace chordalg
