#include "lb_triang_heuristic.h"

namespace chordalg {

LBTriangHeuristic::LBTriangHeuristic( ColoredIntersectionGraph& H) : EliminationOrdering( H ),
    H_( H ),
    B_( H ),
    fill_of_ith_vertex_( H_.order() ),
    separated_of_ith_vertex_( H_.order() ),
    fill_cost_( 0 ),
    fill_weight_( 0 ),
    separated_weight_( 0 )
{
    Unfill( 0, H_.order() - 1 );
    Init();
    return;
}

LBTriangHeuristic::~LBTriangHeuristic()
{
    return;
}

void LBTriangHeuristic::Init()
{
    int n = H_.order();
    alpha_.resize( n );
    alpha_inverse_.resize( n );

    for( Vertex v : H_ )
        alpha_inverse_[ v ] = n;  // So we can use MonotoneAdjacencySet()

    std::map< Vertex, bool > remaining_vertices;
    for( Vertex v : H_ )
        remaining_vertices[ v ] = true;

    // Monochromatic pair costs
    for( Vertex v : H_ )
    {
        for( Vertex u : H_ )
        {
            if( u != v && H_.IsMonochromatic( u, v ) )
            {
                std::pair< Vertex, Vertex > f = std::pair< Vertex, Vertex >( std::min( u, v ), std::max( u, v ) );
                monochromatic_pairs_[ f ] = H_.CommonColorCount( u, v );
            }
        }
    }

    for( int i = 0; i < 1; ++i ) //G_.order(); ++i )
    {
        std::pair< std::pair< Weight, double >, Vertex > min = Min( remaining_vertices, i );

        fill_cost_ += min.first.first;

        Vertex v_min = min.second;
        alpha_[ i ] = v_min;
        alpha_inverse_[ v_min ] = i;
        remaining_vertices.erase( v_min );

        fill_of_ith_vertex_[ i ] = fill_cost_;
        separated_of_ith_vertex_[ i ] = min.first.second;

        FindSaturatingSets( v_min );
        ForEachSaturatingSet( v_min, &LBTriangHeuristic::Saturate );
        UpdateMonochromaticPairs( v_min );

B_.PrettyPrint();
std::cout << H_.name( v_min ) << " removed " << std::endl;
for( std::pair< std::pair< Vertex, Vertex >, Weight > p : monochromatic_pairs_ )
{
    std::cout << p.first.first << ',' << p.first.second << ": " << p.second << std::endl;
}
    }

}

void LBTriangHeuristic::Saturate( Vertex u, Vertex w )
{
    if( fill_edge_count_[ u ][ w ] == 0 )
    {
        fill_size_++;
        fill_neighbors_[ u ][ w ] = fill_neighbors_[ w ][ u ] = true;
    }

    fill_edge_count_[ u ][ w ]++;
    fill_edge_count_[ w ][ u ]++;
    return;
}

void LBTriangHeuristic::UpdateMonochromaticPairs( Vertex v )
{
    std::vector< std::pair< Vertex, Vertex > > to_erase;
    for( std::pair< std::pair< Vertex, Vertex >, Weight > p : monochromatic_pairs_ )
    {
        std::pair< Vertex, Vertex > f = p.first;
        Vertex u = f.first, w = f.second;

        if( ( u == v ) || ( w == v ) )
        {
            to_erase.push_back( f );
        } // if one u, w is v, it must be separated from the other ( monochromatic_pairs_ are separated pairs )
        else if( B_.IsInSeparator( u ) && B_.IsInSeparator( w ) )
        {
            to_erase.push_back( f );
        } // if fill edge
        if( B_.AreSeparated( u, w ) )
            to_erase.push_back( f );
    }

    for( std::pair< Vertex, Vertex > f : to_erase )
        monochromatic_pairs_.erase( f );

    return;
}

void LBTriangHeuristic::FindSaturatingSets( Vertex v )
{
    VertexContainer closed_nbhd;
    closed_nbhd.push_back( v );

    for( Vertex u : G_.N( v ) )
    {
        if( alpha_inverse_[ v ] < alpha_inverse_[ u ] )
            closed_nbhd.push_back( u );
    }

    for( std::pair< Vertex, bool > p : fill_neighbors_[ v ] )
    {
        Vertex u = p.first;
        if( alpha_inverse_[ v ] < alpha_inverse_[ u ] )
            closed_nbhd.push_back( u );
    }

    B_.Separate( closed_nbhd );
    return;
}

void LBTriangHeuristic::MinBody( Vertex u, Vertex v )
{
    std::pair< Vertex, Vertex > f = std::pair< Vertex, Vertex >( std::min( u, v ), std::max( u, v ) );
    if( fill_from_v_.find( f ) == fill_from_v_.end() )
    {
        fill_from_v_.insert( f );
        fill_weight_ += H_.CommonColorCount( u, v );
    }

    return;
}

void LBTriangHeuristic::CountSeparatedMonochromaticPair( Vertex v )
{
    for( std::pair< std::pair< Vertex, Vertex >, Weight > p : monochromatic_pairs_ )
    {
        std::pair< Vertex, Vertex > f = p.first;
        Weight f_cost = p.second;

        if( fill_from_v_.find( f ) == fill_from_v_.end() )
        {
            Vertex u = f.first, w = f.second;
            ConnectedComponentID Cu = B_.ComponentId( u ), Cw = B_.ComponentId( w );

            if( B_.AreSeparated( u, w ) )
                    separated_weight_ += f_cost;
            else if( ( u == v && Cw != B_.kInSeparator() )
                || ( w == v && Cu != B_.kInSeparator() ) )
            {
                separated_weight_ += f_cost;
            } // if either u or w is v, and the other vertex is not in N[v]
        }
    }

    return;
}

std::pair< std::pair< Weight, double >, Vertex > LBTriangHeuristic::Min( std::map< Vertex, bool >& remaining_vertices, int elimination_index )
{
    std::pair< std::pair< Weight, double >, Vertex > min_triple;
    min_triple.first.second = DBL_MAX;

    for( std::pair< Vertex, bool > p : remaining_vertices )
    {
        Vertex v = p.first;
        alpha_inverse_[ v ] = elimination_index;

        fill_weight_ = separated_weight_ = 0;
        fill_from_v_.clear();

        FindSaturatingSets( v );
        ForEachSaturatingSet( v, &LBTriangHeuristic::MinBody );
        CountSeparatedMonochromaticPair( v );

        double cost = fill_weight_ / ( 1 + separated_weight_ );
        if( cost <= min_triple.first.second )
        {
            // if( cost == min_cost_pair.first ) tiebreak
            min_triple.first.first = fill_weight_;
            min_triple.first.second = cost;
            min_triple.second = v;
        }
DEBUG_VARIABLE("penalty / reward", cost)
DEBUG_VARIABLE("# of colors", fill_weight_)
DEBUG_VARIABLE("vertex", v)

        alpha_inverse_[ v ] = H_.order();
    }

    return min_triple;
}

} // namespace chordalg
