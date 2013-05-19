#include "separator.h"

namespace chordalg {

////////////// ctor & dtors
//
SeparatorComponents::SeparatorComponents( Graph const& G ) :
    G_                  ( G         ),
    S_                  ( G.order() ),
    connected_component_(           ),
    search_queue_       (           ),
    size_               ( 0         )
{
    int n = G.order();

    // allocate space
    S_.reserve( n );
    connected_component_.resize( n );

    search_queue_.reserve( n );
    return;
}

SeparatorBlocks::SeparatorBlocks( Graph const& G ) :
    SeparatorComponents( G ),
    neighborhoods_(),
    last_separator_vertex_seen_()
{
    int n = G.order();
    last_separator_vertex_seen_.resize( n );
    return;
}

////////////// Methods
// Main computation
void SeparatorComponents::Separate( Vertices const & S, FillSet& fill )
{
    size_ = 0;

    // intialize separator
    S_.clear();
    for( Vertex v : S )
        S_.add( v );

    // initialize connected component of each vertex
    for( Vertex v : G_ )
        connected_component_[ v ] = kUnsearched();
    for( Vertex v : S )
        connected_component_[ v ] = kInSeparator();

    // main calculation
    FindComponents( fill );
    return;
}

// Finds connected components
void SeparatorComponents::FindComponents( FillSet& fill )
{
    ConnectedComponentID current_component = kUnsearched();
    for( Vertex v : G_ )
    {

        if( IsUnsearched( v ) && !IsInSeparator( v ) )
        {
            search_queue_.push_back( v );
            ++current_component;
            connected_component_[ v ] = current_component;
        } // if new connected component found

        while( !search_queue_.empty() )
        {
            Vertex u = search_queue_.back(); search_queue_.pop_back();
            const Vertices& neighborhood = GetNeighborhood( u, fill );

            // BFS
            for( Vertex w : neighborhood )
            {
                if( IsUnsearched( w ) && !IsInSeparator( w ) )
                {
                    search_queue_.push_back( w );
                    connected_component_[ w ] = current_component;
                } // if w is unsearched and not in S
            }
        }
    }

    size_ = current_component + 1;
    return;
}

// Finds connected component containing v
Vertices SeparatorComponents::ConnectedComponent( Vertex v ) const
{
    Vertices C( 0 );
    if( IsInSeparator( v ) )
        return C;

    for( Vertex u : G_ )
    {
        if( AreConnected( u, v ) )
            C.add( u );
    }
    return C;
}

Vertices SeparatorComponents::GetNeighborhood( Vertex u, FillSet& fill )
{

    if( fill.empty() )
        return Vertices( G_.N( u ) );
    else
    {
        Vertices neighborhood( 0 );

        for( Vertex v : G_.N( u ) )
            neighborhood.add( v );

        for( Vertex v : fill[ u ] )
            neighborhood.add( v );

        return neighborhood;
    }
}

void SeparatorBlocks::Separate( Vertices const & S, FillSet& fill )
{
    SeparatorComponents::Separate( S, fill );

    neighborhoods_.clear();
    neighborhoods_.resize( size_ );
    last_separator_vertex_seen_.clear();

    // main computation
    FindNeighborhoods( fill );

    return;
}

// Finds the neighborhoods of connected components.
// Our implementation is an extension of the algorithm found on p.50 in the paper below.
// paper: http://www.sciencedirect.com/science/article/pii/S0196677404001142
// citation: http://www.informatik.uni-trier.de/~ley/db/journals/jal/jal58.html#BerryBHSV06
void SeparatorBlocks::FindNeighborhoods( FillSet& fill )
{

    // used to ensure each element of N(C) appears once
    last_separator_vertex_seen_.resize( size_ );
    for( ConnectedComponentID &C : last_separator_vertex_seen_ )
        C = -1;

    for( Vertex v : S_ )
    {
        const Vertices& neighborhood = GetNeighborhood( v, fill );

        for( Vertex u : neighborhood )
        {
            ConnectedComponentID C = connected_component_[ u ];
            if( !IsInSeparator( u ) && last_separator_vertex_seen_[ C ] != v )
            {
                neighborhoods_[ C ].add( v );
                last_separator_vertex_seen_[ C ] = v;
            } // if u is not a separator vertex, u is in C, and we haven't determined that v is in N(C)
        }
    }

    return;
}

void SeparatorComponents::PrettyPrint()
{
    for( Vertex v : G_ )
        std::cout << "CC(" << G_.name( v ) << "): " << connected_component_[ v ] << std::endl;
    std::cout << std::endl;

    return;
}

void SeparatorBlocks::PrettyPrint()
{
    SeparatorComponents::PrettyPrint();
    ConnectedComponentID cc = 0;

    for( Vertices NC : *this )
    {
        std::cout << "N(C_" << cc << "): ";
        for( Vertex v : NC )
            std::cout << G_.name( v ) << " ";
        std::cout << std::endl;
        ++cc;
    }

    return;
}

} // namespace chordalg


