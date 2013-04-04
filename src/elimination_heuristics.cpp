#include "elimination_heuristics.h"

namespace chordalg {

MonochromaticFillPairHeuristic::MonochromaticFillPairHeuristic( ColoredIntersectionGraph& H ) : EliminationOrdering( H ),
    H_( H ),
    fill_weight_( 0 )
{
    Unfill( 0, H_.order() - 1 );
    Init();
    return;
}

MonochromaticFillPairHeuristic::~MonochromaticFillPairHeuristic()
{
    return;
}

void MonochromaticFillPairHeuristic::Init()
{
    int n = H_.order();
    alpha_.resize( n );
    alpha_inverse_.resize( n );

    for( Vertex v : H_ )
        alpha_inverse_[ v ] = n;  // So we can use MonotoneAdjacencySet()

    std::map< Vertex, bool > remaining_vertices;
    for( Vertex v : H_ )
        remaining_vertices[ v ] = true;

    for( int i = 0; i < H_.order(); ++i )
    {
        std::pair< Weight, Vertex > min = Min( remaining_vertices, i );

        fill_weight_ += min.first;

        Vertex v_min = min.second;
        alpha_[ i ] = v_min;
        alpha_inverse_[ v_min ] = i;
        remaining_vertices.erase( v_min );

        Refill( i, i );
    }
    return;
}

void MonochromaticFillPairHeuristic::MinBody( Vertex u, Vertex w )
{
    if( !IsFillEdge( u, w ) )
    {
        Multicolor intersection;
        std::set_intersection(  H_.vertex_color( u ).begin(), H_.vertex_color( u ).end(),
                                H_.vertex_color( w ).begin(), H_.vertex_color( w ).end(),
                                std::inserter( intersection, intersection.begin() ) );

        monochromatic_fill_pairs_count_ += intersection.size();
    }
    return;
}

std::pair< Weight, Vertex > MonochromaticFillPairHeuristic::Min( std::map< Vertex, bool >& remaining_vertices, int elimination_index )
{
    std::pair< Weight, Vertex > min_pair;
    min_pair.first = MAX_WEIGHT;

    for( std::pair< Vertex, bool > p : remaining_vertices )
    {
        Vertex v = p.first;
        alpha_inverse_[ v ] = elimination_index;

        monochromatic_fill_pairs_count_ = 0;
        EliminationOrdering::ForEachFillPair( v, &MonochromaticFillPairHeuristic::MinBody );
        min_pair = std::min( min_pair, std::pair< Weight, Vertex >( monochromatic_fill_pairs_count_, v ) );

        alpha_inverse_[ v ] = H_.order();
    }

    return min_pair;
}


} // namespace chordalg

