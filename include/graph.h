/*
 *  graph.h - a graph data structure
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <map>
#include <sstream>

#include "chordalg_types.h"
#include "file_reader.h"
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
        void PrettyPrint() const;

        GVIterator              begin   ( )                       const { return GraphVertices( this, order_ ).begin(); }
        GVIterator              end     ( )                       const { return GraphVertices( this, order_ ).end();   }

        Vertex                  vertex  ( char id               ) const;
        Vertex                  vertex  ( std::string id        ) const;
        const VertexName&       name    ( Vertex v              ) const { return vertex_names_->operator[]( v );        }
        int                     order   ( )                       const { return order_;                                }
        int                     size    ( )                       const { return size_;                                 }

        bool                    HasEdge ( Vertex u, Vertex v    ) const { return is_edge_[ u ][ v ];                    }
        bool                    HasEdge ( VertexPair p          ) const { return HasEdge( p.first, p.second );          }
        template< class Container >
        bool                    HasClique( Container set )        const { return HasClique( set.begin(), set.end() );   }
        bool                    IsClique ( )                      const { return 2*size_ == order_ * ( order_ - 1 );    }
        const Vertices&         N        ( Vertex v             ) const { return neighborhoods_->operator[]( v );       }

    protected:
        void Init();

        const AdjacencyLists*               neighborhoods_;
        const VertexNames*                  vertex_names_;
        std::map<VertexName, Vertex>        vertex_ids_;

        // TODO: try changing to std::set< VertexPair >, time experiments otherwise for thousands of vertices this is huge..
        std::vector< std::vector< bool > >  is_edge_;

        unsigned int                        order_,         // #vertices
                                            size_;          // #edges

        // Induced Subgraph Initialization
        VertexNames*        InducedNames    ( Graph&, Vertices                          );
        AdjacencyLists*     InducedVertices ( Graph&, Vertices                          );
        VertexNames*        DefaultNames    ( size_t                                    );

        template< class InputIterator >
        bool                HasClique       ( InputIterator begin, InputIterator end    ) const ;

        // Disable default constructor, copy constructor, assignment
        Graph();
        Graph(const Graph&);
        void operator=(const Graph&);
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
