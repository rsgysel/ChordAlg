#ifndef VERTEX_UTILITIES_H
#define VERTEX_UTILITIES_H

#include <list>
#include <set>
#include <vector>

#include "utilities.h"

namespace chordalg {

// Forward declerations
class   Graph;

// Types

typedef int                             Vertex;
typedef std::pair< Vertex, Vertex >     VertexPair;

typedef std::list   < Vertex >          VertexList;
typedef std::set    < Vertex >          VertexSet;
typedef std::vector < Vertex >          VertexVector;

typedef VertexList::iterator            VLIterator;
typedef VertexList::const_iterator      VLConstItr;
typedef VertexSet::iterator             VSIterator;
typedef VertexSet::const_iterator       VSConstItr;
typedef VertexVector::iterator          VVIterator;
typedef VertexVector::const_iterator    VVConstItr;

typedef VertexVector                    Nbhd;
typedef std::vector< Nbhd >             AdjacencyLists;

typedef std::string                     VertexName;
typedef std::vector< std::string >      VertexNames;

// elimination ordering typedefs
typedef double                          Weight;         // argument to minimize
typedef double                          Cost;           // price for fill edge
typedef std::pair< Vertex, Cost >       VertexCost;


class Vertices
{
    public:
        Vertices    ( int n             )                   { V_.resize( n ); return;   }
        Vertices    ( Vertices& other   ) : V_( other.V_ )  {                           }

        Vertex& operator[]  ( int i )   { return V_[ i ];                                       }
        void    clear       ( )         { V_.clear();                                           }
        int     size        ( ) const   { return V_.size();                                     }
//        void    PrettyPrint ( ) const   { TPrettyPrint< VertexVector, Vertex >( V_ ); return;   }

        VertexVector::iterator          begin   ( )         { return V_.begin(); }
        VertexVector::const_iterator    begin   ( ) const   { return V_.begin(); }
        VertexVector::iterator          end     ( )         { return V_.end();   }
        VertexVector::const_iterator    end     ( ) const   { return V_.end();   }

    private:
        VertexVector V_;
};  // Vertices

class GVIterator
{
    public:
        GVIterator( const Graph* const graph_id             )   : graph_id_( graph_id ), v_( 0 ) {};
        GVIterator( const Graph* const graph_id, Vertex v   )   : graph_id_( graph_id ), v_( v ) {};

        void     operator++ ( )                                 { ++v_;                         }
        Vertex   operator*  ( )                         const   { return v_;                    }
        bool     operator!= ( const GVIterator& other ) const   { return !( *this == other );   }
        bool     operator== ( const GVIterator& other ) const
            { return ( graph_id_ == other.graph_id_ ) && ( v_ == other.v_ );                    }

    private:
        const Graph* const  graph_id_;
        Vertex              v_;
};  // GraphVertexIterator

class GraphVertices : public Vertices
{
    public:
        GraphVertices ( const Graph* const G, int order ) : Vertices( 0 ), G_( G ), order_( order ) {}

        inline GVIterator begin()   const   { return GVIterator( G_ );          }
        inline GVIterator end()     const   { return GVIterator( G_, order_ );  }
    private:
        const Graph* const  G_;
        int                 order_;
};  // GraphVertices

class VertexPairs;

class VertexPairsIterator
{
    public:
        VertexPairsIterator( const VertexVector* const, int, int );

        void        operator++  ( );
        bool        operator==  ( const VertexPairsIterator& other ) const
                {   return ( V_ == other.V_ ) && ( v1_ == other.v1_ ) && ( v2_ == other.v2_ ); }
        bool        operator!=  ( const VertexPairsIterator& other ) const
                {   return !( *this == other ); }
        VertexPair  operator*   ( )
                {   return VertexPair( V_->operator[]( v1_ ), V_->operator[]( v2_ ) ); }

    private:
        const VertexVector* const V_;
        int v1_, v2_;
};  // VertexPairsIterator

class VertexPairs
{
    public:
        VertexPairs( VertexVector    V ) : V_( V ), begin_( 0 ), end_( V.size() ) {}
        //VertexPairs( Graph              G ) : begin_( G.begin() ), end_( G.end() ) {}

        VertexPairsIterator begin    ()  const ;
        VertexPairsIterator end      ()  const ;

    private:
        const VertexVector  V_;
        int                 begin_,
                            end_;
};  // VertexPairs

} // namespace chordalg

#endif // VERTEX_UTILITIES_H
