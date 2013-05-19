#include "elimination_order.h"

namespace chordalg {

EliminationOrder::EliminationOrder( Graph& G ) :
    G_          ( G         ),
    alpha_      ( G.order() ),
    fill_count_ ( 0         ),
    fill_cost_  ( 0         )
{
    srand( time(NULL) );
    return;
}

EliminationOrder::~EliminationOrder()
{
    return;
}

void EliminationOrder::Init()
{
    int n = G_.order();

    alpha_inverse_.resize   ( n );
    fill_neighbors_.resize  ( n );
    tie_count_.resize       ( n );

    for( Vertex v : G_ )
        remaining_vertices_.insert( v );

    Elimination();

    return;
}

void EliminationOrder::Elimination()
{
    for( int i = 0; i < G_.order(); ++i )
    {
        VertexCost vc   =   ArgMin();
        fill_cost_      +=  vc.second;
        Vertex v        =   vc.first;

        alpha_          [ i ] = v;
        alpha_inverse_  [ v ] = i;

        tie_count_      [ i ] = ties_.size();
        ties_.clear();

        remaining_vertices_.erase   ( v );
        Eliminate                   ( v );
    }

    return;
}

void EliminationOrder::AddEdge( VertexPair p )
{
    if( !IsEdge( p ) )
    {
        fill_neighbors_[ p.first  ].insert( p.second );
        fill_neighbors_[ p.second ].insert( p.first  );
        ++fill_count_;
    }

    return;
}

bool EliminationOrder::IsEdge( VertexPair p )
{
    return G_.HasEdge ( p ) || IsFillEdge( p );
}

bool EliminationOrder::IsFillEdge( VertexPair p )
{
    return fill_neighbors_[ p.first ].find( p.second ) != fill_neighbors_[ p.first ].end();
}

bool EliminationOrder::IsRemoved( Vertex v )
{
    return remaining_vertices_.find( v ) == remaining_vertices_.end();
}

void EliminationOrder::Saturate( Vertices U )
{
    for( VertexPair p : VertexPairs( U ) )
    {
        if( !IsEdge( p ) )
        {
            Vertex  u = p.first,
                    v = p.second;
            fill_neighbors_[ u ].insert( v );
            fill_neighbors_[ v ].insert( u );
        }
    }

    return;
}

VertexCost EliminationOrder::ArgMin()
{
    Weight min = MAX_WEIGHT;

    for( Vertex v : remaining_vertices_ )
    {
        std::pair< Weight, Cost >   wc = WeightOf( v );
        Weight                      wt = wc.first;
        Cost                        c  = wc.second;

        if( wt < min )
        {
            min = wt;
            ties_.clear();
            ties_.push_back( VertexCost( v, c ) );
        }
        else if( wt == min )
        {
            ties_.push_back( VertexCost( v, c ) );
        }
    }

    return TieBreak();
}

VertexCost EliminationOrder::TieBreak()
{
    int i = rand() % ties_.size();
    return ties_[ i ];
}

Vertices EliminationOrder::MonotoneNbhd( Vertex v )
{
    Vertices N_alpha;

    for( Vertex u : G_.N( v ) )
    {
        if( !IsRemoved( u ) )
            N_alpha.add( u );
    }

    for( Vertex u : fill_neighbors_[ v ] )
    {
        if( !IsRemoved( u ) )
            N_alpha.add( u );
    }

    return N_alpha;
}

} // namespace chordalg
