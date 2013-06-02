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

class Graph
{
    public:
        Graph   ( Graph& H                      );
        Graph   ( FileReader*                   );
        Graph   ( AdjacencyLists*               );
        Graph   ( AdjacencyLists*,  VertexNames );
        Graph   ( Graph&,           Vertices    );
        virtual ~Graph();

        bool IsIsomorphic( Graph& );
        void PrettyPrint();

        GVIterator              begin   ( )                       const { return GraphVertices( this, order_ ).begin(); }
        GVIterator              end     ( )                       const { return GraphVertices( this, order_ ).end();   }

        const VertexName&       name    ( Vertex v              ) const { return vertex_names_->operator[]( v );        }
        int                     order   ( )                       const { return order_;                                }
        int                     size    ( )                       const { return size_;                                 }

        bool                    HasEdge ( Vertex u, Vertex v    ) const { return is_edge_[ u ][ v ];                    }
        bool                    HasEdge ( VertexPair p          ) const { return HasEdge( p.first, p.second );          }
        template< class Container >
        bool                    HasClique( Container set )        const { return HasClique( set.begin(), set.end() );   }
        bool                    IsClique ( )                      const { return 2*size_ == order_ * ( order_ - 1 );    }
        const Vertices&         N       ( Vertex v              ) const { return neighborhoods_->operator[]( v );       }
// REQUIRES DEBUGGING //        const Vertices&         V       (                       ) const { return GraphVertices( this, order_ );         }


    protected:
        void Init();

        const AdjacencyLists*               neighborhoods_;
        const VertexNames*                  vertex_names_;
        std::vector< std::vector< bool > >  is_edge_;

        unsigned int                        order_,         // #vertices
                                            size_;          // #edges

        // Induced Subgraph Initialization
        VertexNames*        InducedNames    ( Graph&, Vertices                          );
        AdjacencyLists*     InducedVertices ( Graph&, Vertices                          );
        VertexNames*        DefaultNames    ( size_t                                    );

        template< class InputIterator >
        bool                HasClique       ( InputIterator begin, InputIterator end    ) const ;

        DISALLOW_DEFAULT_CONSTRUCTOR        ( Graph                                     );
        DISALLOW_COPY_AND_ASSIGN            ( Graph                                     );
};  // Graph

////////////// Generics
//

template< class InputIterator >
bool Graph::HasClique( InputIterator begin, InputIterator end ) const
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
