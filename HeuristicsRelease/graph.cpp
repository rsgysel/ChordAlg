#include "graph.h"

namespace chordalg{

////////////// Graph
////////////// ctor & dtors
//
Graph::Graph( Graph& H ) :
    neighborhoods_  (   new AdjacencyLists      (   *( H.neighborhoods_ )   )   ),
    vertex_names_   (   new VertexNames         (   *( H.vertex_names_  )   )   ),
    order_          (   H.order_    ),
    size_           (   H.size_     )
{
    Init();
    return;
}

Graph::Graph( FileReader* fr ) :
    neighborhoods_  (   fr->TakeNeighborhoods() ),
    vertex_names_   (   fr->TakeNames()         ),
    order_          (   neighborhoods_->size()  ),
    size_           (   0   )
{
    Init();
    return;
}

Graph::Graph( AdjacencyLists* a_lists ) :
    neighborhoods_  (   a_lists                         ),
    vertex_names_   (   DefaultNames( a_lists->size() ) ),
    order_          (   a_lists->size()                 ),
    size_           (   0   )
{
    Init();
    return;
}

Graph::Graph( AdjacencyLists* a_lists, VertexNames vertex_names ) :
    neighborhoods_  (   a_lists                             ),
    vertex_names_   (   new VertexNames ( vertex_names )    ),
    order_          (   a_lists->size   ( )                 ),
    size_           (   0                                   )
{
    Init();
    return;
}

Graph::Graph( Graph& super_graph, Vertices X ) :
    neighborhoods_  (   InducedVertices ( super_graph, X )  ),
    vertex_names_   (   InducedNames    ( super_graph, X )  ),
    order_          (   X.size          ( )                 ),
    size_           (   0                                   )
{
    Init();
    return;
}

Graph::~Graph()
{
    delete neighborhoods_;
    delete vertex_names_;
}

void Graph::Init()
{
    for( Nbhd const& N : *neighborhoods_ )
        size_ += N.size();
    size_ /= 2;

    // adjacency matrix
    is_edge_.resize( order_ );
    for( std::vector< bool >& V : is_edge_ )
        V.resize( order_ );

    for( Vertex x : *this )
    {
        for( Vertex y : *this )
            is_edge_[ x ][ y ] = false;
    }

//    for( VertexPair uv : VertexPairs( this->V() ) )
//        is_edge_[ uv.first ][ uv.second ] = is_edge_[ uv.second ][ uv.first ] = false;

    for( Vertex x : *this )
    {
        for( Vertex y : this->N( x ) )
            is_edge_[ x ][ y ] = true;
    }

    // check consistency
    for( Vertex x : *this )
    {
        for( Vertex y : *this )
        {
            if( is_edge_[ x ][ y ] != is_edge_[ y ][ x ] )
            {
                std::cerr << "Error: edge relationships not symmetric wrt " << name( x ) << " and " << name( y ) << std::endl;
                exit( 0 );
            }
        }
    }
//    for( VertexPair uv : VertexPairs( this->V() ) )
//    {
//        if( is_edge_[ uv.first ][ uv.second ] != is_edge_[ uv.second ][ uv.first ] )
//        {
//            std::cerr << "Error: edge relationships not symmetric wrt " << name( uv.first )
//                            << " and " << name( uv.second ) << std::endl;
//            exit( 0 );
//        }
//    }
    return;
}

VertexNames* Graph::InducedNames( Graph& super_graph, Vertices X )
{
    std::sort( X.begin(), X.end() );

    VertexNames* names = new VertexNames;
    names->reserve( X.size() );

    for( Vertex v : X )
        names->push_back( super_graph.name( v ) );

    return names;
}

AdjacencyLists* Graph::InducedVertices( Graph& super_graph, Vertices X )
{
    std::sort( X.begin(), X.end() );

    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize( X.size() );

    std::map< Vertex, int > vertex_order;
    int i = 0;
    for( Vertex v : X )
    {
        vertex_order[ v ] = i;
        ++i;
    }

    std::vector< bool > in_subgraph;
    in_subgraph.resize( super_graph.order() );

    for( Vertex v : super_graph )
        in_subgraph[ v ] = false;
    for( Vertex v : X )
        in_subgraph[ v ] = true;

    for( Vertex v : X )
    {
        for( Vertex u : super_graph.N( v ) )
        {
            if(in_subgraph[ u ])
            {
                Vertex  v_induced_id = vertex_order[ v ],
                        u_induced_id = vertex_order[ u ];
                a_lists->operator[]( v_induced_id ).add( u_induced_id );
            }
        }
    }

    return a_lists;
}

VertexNames* Graph::DefaultNames( size_t order )
{
    int             n       =   order;
    VertexNames*    names   =   new VertexNames();

    names->resize( n );
    for( int i = 0; i < n; ++i )
    {
        std::stringstream s_i;
        s_i << i;
        names->operator[]( i ) = s_i.str();
    }
    return names;
}


// Determines if H is isomorphic to G with respect to their fixed orderings
bool Graph::IsIsomorphic( Graph& H )
{
    Graph& G = *this;
    if( G.order()   !=  H.order() ||
        G.size()    !=  H.size() )
        return false;
    for( Vertex v : G )
    {
        if( G.N( v ).size() != H.N( v ).size() )
            return false;

        if( !std::equal( G.N( v ).begin(), G.N( v ).end(), H.N( v ).begin() ) )
            return false;
    }
    return true;
}

void Graph::PrettyPrint()
{
    std::cout   <<  "Order: "   <<  order_  <<  std::endl;
    std::cout   <<  "Size: "    <<  size_   <<  std::endl;

    for( Vertex v : *this )
    {
        std::cout << "N(" << name( v ) << "): ";
        for( Vertex u : N( v ) )
            std::cout << name( u ) << " ";
        std::cout << std::endl;
    }
    return;
}

}   // namespace chordalg
