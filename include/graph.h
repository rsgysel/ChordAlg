#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <map>
#include <sstream>

#include "chordalg_types.h"
#include "file_reader.h"
#include "utilities.h"
#include "vertex_utilities.h"

namespace chordalg {

class Graph;

class GraphIterator
{
    public:
        GraphIterator( const Graph* const graph_id ) : graph_id_( graph_id ), v_( 0 ) {};
        GraphIterator( const Graph* const graph_id, Vertex v ) : graph_id_( graph_id ), v_( v ) {};

        inline void operator++(){ ++v_; }
        inline bool operator==( const GraphIterator& other ) const
            { return ( graph_id_ == other.graph_id_ ) && ( v_ == other.v_ ); }
        inline bool operator!=( const GraphIterator& other ) const { return !( *this == other ); }
        inline Vertex operator*() const { return v_; }
    private:
        const Graph* const graph_id_;
        Vertex v_;
};  // GraphVertexIterator

class Graph
{
    public:
        Graph( Graph& H );
        Graph( FileReader* );
        Graph( AdjacencyLists* );
        Graph( AdjacencyLists*, VertexNames );
        Graph( Graph&, VertexVector );
        virtual ~Graph();

        bool IsIsomorphic( Graph& );
        void PrettyPrint();

        // stat accessors
        int order() const { return order_; }
        int size() const { return size_; }            // # edges

        // "iteration" accessors
        inline GraphIterator begin()    const   { return GraphIterator( this );         }
        inline GraphIterator end()      const   { return GraphIterator( this, order_ ); }

        // Neighborhood accessor
        inline const VertexVector& N( Vertex v ) const { return neighborhoods_->operator[]( v ); }

        // Edge accessors
        inline bool HasEdge( Vertex u, Vertex v ) { return is_edge_[ u ][ v ];              }
        inline bool HasEdge( VertexPair p       ) { return HasEdge( p.first, p.second );    }

        template< class Container >
        bool HasClique( Container set ) { return HasClique( set.begin(), set.end() ); }
        bool IsClique () { return 2*size_ == order_ * ( order_ - 1 ); }

        // Name accessor
        const VertexName& name( Vertex v ) const { return vertex_names_->operator[]( v ); }

    protected:
        VertexNames* DefaultNames( size_t );
        void Init();
        template< class InputIterator >
        bool HasClique( InputIterator begin, InputIterator end );

        const AdjacencyLists* neighborhoods_;
        const VertexNames* vertex_names_;
        std::vector< std::vector< bool > > is_edge_;

        unsigned int order_, size_;     // #vertices, #edges

        // Induced Subgraph Initialization
        VertexNames* InducedNames( Graph&, VertexVector );
        AdjacencyLists* InducedVertices( Graph&, VertexVector );

        DISALLOW_DEFAULT_CONSTRUCTOR( Graph );
        DISALLOW_COPY_AND_ASSIGN( Graph );
};  // Graph

////////////// Generics
//

template< class InputIterator >
bool Graph::HasClique( InputIterator begin, InputIterator end )
{
    for( InputIterator vertex_itr = begin; vertex_itr != end; ++vertex_itr )
    {
        InputIterator neighbor_itr = vertex_itr;
        for( ++neighbor_itr; neighbor_itr != end; ++neighbor_itr )
        {
            if( !HasEdge( *vertex_itr, *neighbor_itr ) )
                return false;
        }
    }

    return true;
}

} // namespace chordalg

#endif // GRAPH_H
