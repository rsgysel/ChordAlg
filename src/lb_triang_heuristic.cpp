#include "lb_triang_heuristic.h"

namespace chordalg {

LBTriangHeuristic::LBTriangHeuristic( ColoredIntersectionGraph& H ) : EliminationOrdering( H ),
    H_( H ),
    B_( H ),
    deleted_vertices_(),
    fill_of_ith_vertex_( H_.order() ),
    separated_of_ith_vertex_( H_.order() ),
    fill_cost_( 0 ),
    fill_weight_( 0 ),
    separated_weight_( 0 )
{
    srand( time(NULL) );
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

    for( int i = 0; i < G_.order(); ++i )
    {
        std::pair< std::pair< Weight, double >, Vertex > min = Min( remaining_vertices, i );

        fill_cost_ += min.first.first;

        Vertex v_min = min.second;
        alpha_[ i ] = v_min;
        alpha_inverse_[ v_min ] = i;
        remaining_vertices.erase( v_min );

        fill_of_ith_vertex_[ i ] = min.first.first;
        separated_of_ith_vertex_[ i ] = min.first.second;

        FindSaturatingSets( v_min );
        ForEachSaturatingSet( v_min, &LBTriangHeuristic::Saturate );
        UpdateMonochromaticPairs( v_min );

        deleted_vertices_.insert( v_min );
    }

}

void LBTriangHeuristic::Saturate( Vertex u, Vertex w )
{
    if( deleted_vertices_.find( u ) == deleted_vertices_.end()
        && deleted_vertices_.find( w ) == deleted_vertices_.end() )
    {
        if( fill_edge_count_[ u ][ w ] == 0 )
        {
            fill_size_++;
            fill_neighbors_[ u ].insert( w );
            fill_neighbors_[ w ].insert( u );
        }

        fill_edge_count_[ u ][ w ]++;
        fill_edge_count_[ w ][ u ]++;
    } // if neither vertex is deleted

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

    for( Vertex u : fill_neighbors_[ v ] )
    {
        if( alpha_inverse_[ v ] < alpha_inverse_[ u ] )
            closed_nbhd.push_back( u );
    }

    for( Vertex u : deleted_vertices_ )
        closed_nbhd.push_back( u );

    B_.Separate( closed_nbhd, fill_neighbors_ );

    return;
}

void LBTriangHeuristic::MinBody( Vertex u, Vertex v )
{
    if( deleted_vertices_.find( u ) == deleted_vertices_.end()
        && deleted_vertices_.find( v ) == deleted_vertices_.end() )
    {
        std::pair< Vertex, Vertex > f = std::pair< Vertex, Vertex >( std::min( u, v ), std::max( u, v ) );
        if( fill_from_v_.find( f ) == fill_from_v_.end() )
        {
            fill_from_v_.insert( f );
            fill_weight_ += H_.CommonColorCount( u, v );
        }
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
        if( cost < min_triple.first.second ||
            ( cost == min_triple.first.second && ( rand()%2 == 0 ) ) )
        {
            min_triple.first.first = fill_weight_;
            min_triple.first.second = cost;
            min_triple.second = v;
        }

        alpha_inverse_[ v ] = H_.order();
    }

    return min_triple;
}

void LBTriangHeuristic::PrettyPrint()
{
    std::cout << "elimination order: " << std::endl;
    for( int i = 0; i < alpha_.size(); ++i )
        std::cout << H_.name( alpha_[ i] ) << ": fill " << fill_of_ith_vertex_[ i ] << " and cost " << separated_of_ith_vertex_[ i ] << std::endl;
    std::cout << std::endl;

    EliminationOrdering::PrettyPrint();
    return;
}

} // namespace chordalg
