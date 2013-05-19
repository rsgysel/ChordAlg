#include "separation_heuristics.h"

namespace chordalg {

MonochromaticPDRS::MonochromaticPDRS(  ColoredIntersectionGraph& H ) :
    EliminationOrder    ( H ),
    H_                  ( H ),
    B_                  ( H )
{
    MonochromaticPDRS::Init();
    return;
}

MonochromaticPDRS::~MonochromaticPDRS()
{
    return;
}

void MonochromaticPDRS::Init()
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

    EliminationOrder::Init();

    return;
}

void MonochromaticPDRS::Eliminate( Vertex v )
{
    Vertices S = MonotoneNbhd( v );
    S.add( v );
    B_.Separate( S, fill_neighbors_ );

<<<<<<< HEAD
    for( Vertices NC : B_ )
=======
    for( VertexVector NC : B_ )
>>>>>>> 137a14401e4ae061416a9d50dabfd29c7da40d9d
    {
        for( VertexPair uv : VertexPairs( NC ) )
        {
            AddEdge( uv );
            unseparated_monochromatic_pairs_.erase( uv );
        }
    }

    return;
}

std::pair< Weight, Cost > MonochromaticPDRS::WeightOf( Vertex v )
{
    Weight  deficiency_wt   = 0,
            separated_wt    = 0;

<<<<<<< HEAD
    Vertices S = MonotoneNbhd( v );
    S.add( v );
=======
    VertexVector S = MonotoneNbhd( v );
    S.push_back( v );
>>>>>>> 137a14401e4ae061416a9d50dabfd29c7da40d9d
    B_.Separate( S , fill_neighbors_ );

    // monochromatic fill pairs
    std::set< VertexPair > seen_fill_pairs;
<<<<<<< HEAD
    for( Vertices NC : B_ )
=======
    for( VertexVector NC : B_ )
>>>>>>> 137a14401e4ae061416a9d50dabfd29c7da40d9d
    {
        for( VertexPair uw : VertexPairs( NC ) )
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

    return ObjectiveFunction( deficiency_wt, separated_wt );
}

std::pair< Weight, Cost > MonochromaticPDRS::ObjectiveFunction( Weight deficiency_wt, Weight separated_wt )
{
    return std::pair< Weight, Cost >(   deficiency_wt / ( 1 + separated_wt ) ,
                                        deficiency_wt );
}

} // namespace chordalg
