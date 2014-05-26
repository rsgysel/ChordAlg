#include "lb_elimination.h"

namespace chordalg {

LBElimination::LBElimination(  ColoredIntersectionGraph& H, LBCriterion* f ) :
    EliminationAlgorithm    ( H ),
    H_                  ( H ),
    B_                  ( H ),
    f_                  ( f )
{
    LBElimination::Init();
    return;
}

LBElimination::~LBElimination()
{
    return;
}

void LBElimination::Init()
{
    // Monochromatic pair costs
    for( Vertex v : H_ )
    {
        for( Vertex u : H_ )
        {
            if( u != v && H_.IsMonochromatic( u, v ) )
            {
                VertexPair uv = VertexPair( std::min( u, v ), std::max( u, v ) );
                unseparated_monochromatic_pairs_[ uv ] = H_.CommonColorCount( u, v );
            }
        }
    }

    EliminationAlgorithm::Init();

    return;
}

void LBElimination::Eliminate( Vertex v )
{
    Vertices S = MonotoneNbhd( v );
    S.add( v );
    B_.Separate( S, fill_neighbors_ );

    for( Block B : B_ )
    {
        for( VertexPair uv : VertexPairs( B.NC() ) )
        {
            AddEdge( uv );
            unseparated_monochromatic_pairs_.erase( uv );
        }
    }

    return;
}

std::pair< Weight, Cost > LBElimination::WeightOf( Vertex v )
{
    Weight  deficiency_wt   = 0,
            separated_wt    = 0;

    Vertices S = MonotoneNbhd( v );
    S.add( v );
    B_.Separate( S , fill_neighbors_ );

    // monochromatic fill pairs
    std::set< VertexPair > seen_fill_pairs;
    for( Block B : B_ )
    {
        for( VertexPair uw : VertexPairs( B.NC() ) )
        {
            Cost fill_cost = H_.CommonColorCount( uw.first, uw.second );
            if( !IsEdge( uw ) &&
                fill_cost > 0 &&
                seen_fill_pairs.find( uw ) == seen_fill_pairs.end() )
            {
                deficiency_wt += fill_cost;
                seen_fill_pairs.insert( uw );
            }
        }
    }

    // new monochromatic separation
    for( std::pair< VertexPair, Weight > p : unseparated_monochromatic_pairs_ )
    {
        VertexPair              uw          = p.first;
        Vertex                  u           = uw.first,
                                w           = uw.second;
        Weight                  fill_cost   = p.second;

        if( B_.AreSeparated( u, w ) )
            separated_wt += fill_cost;
    }

    return std::pair< Weight, Cost>( f_->Calculate( deficiency_wt, separated_wt ),
                                     deficiency_wt );
}

std::pair< Weight, Cost > LBElimination::ObjectiveFunction( Weight deficiency_wt, Weight separated_wt )
{
    return std::pair< Weight, Cost >(   deficiency_wt / ( 1 + separated_wt ),
                                        deficiency_wt );
}

} // namespace chordalg
