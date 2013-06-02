#ifndef VERTEX_UTILITIES_H
#define VERTEX_UTILITIES_H

#include <list>
#include <set>
#include <vector>

#include "utilities.h"

namespace chordalg {

// Forward declerations
class                                   Graph;
class                                   Vertices;

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

typedef Vertices                        Nbhd;
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
        typedef VertexVector::const_iterator    const_iterator;

        Vertices    ( )                                                                 { return;                       }
        Vertices    ( int n                             )                               { V_.resize( n, 0 ); return;    }
        Vertices    ( const Vertices& other             ) : V_( other.V_ )              { return;                       }
        Vertices    ( std::initializer_list< Vertex > V ) : V_( V )                     { return;                       }
        Vertices    ( VertexList                      V ) : V_( V.begin(), V.end() )    { return;                       }
        Vertices    ( VertexSet                       V ) : V_( V.begin(), V.end() )    { return;                       }
        Vertices    ( VertexVector                    V ) : V_( V )                     { return;                       }

        void            add         ( Vertex v   )          { V_.push_back( v );    }
        bool            empty       ( )             const   { return V_.empty();    }
        Vertex&         operator[]  ( int i      )          { return V_[ i ];       }
        const Vertex&   operator[]  ( int i      )  const   { return V_[ i ] ;      }
        void            clear       ( )                     { V_.clear();           }
        void            reserve     ( int n      )          { V_.reserve( n );      }
        int             size        ( )             const   { return V_.size();     }

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
        GVIterator( const Graph* const G            )   : G_( G ), v_( 0 ) {};
        GVIterator( const Graph* const G, Vertex v  )   : G_( G ), v_( v ) {};

        void     operator++ ( )                                 { ++v_;                         }
        Vertex   operator*  ( )                         const   { return v_;                    }
        bool     operator!= ( const GVIterator& other ) const   { return !( *this == other );   }
        bool     operator== ( const GVIterator& other ) const
            { return ( G_ == other.G_ ) && ( v_ == other.v_ );                    }

    private:
        const Graph* const  G_;
        Vertex              v_;
};  // GraphVertexIterator

class GraphVertices : public Vertices
{
    public:
        GraphVertices ( const Graph* const G, int order ) : Vertices( ), G_( G ), order_( order ) {}

        GVIterator begin()   const   { return GVIterator( G_ );          }
        GVIterator end()     const   { return GVIterator( G_, order_ );  }

    private:
        const Graph* const  G_;
        int                 order_;
};  // GraphVertices

class VertexPairs;

class VertexPairsIterator
{
    public:
        VertexPairsIterator( const Vertices* const, int, int );

        void        operator++  ( );
        bool        operator==  ( const VertexPairsIterator& other ) const
                {   return ( V_ == other.V_ ) && ( v1_ == other.v1_ ) && ( v2_ == other.v2_ ); }
        bool        operator!=  ( const VertexPairsIterator& other ) const
                {   return !( *this == other ); }
        VertexPair  operator*   ( )
                {   return VertexPair( V_->operator[]( v1_ ), V_->operator[]( v2_ ) ); }

    private:
        const Vertices* const V_;
        int v1_, v2_;
};  // VertexPairsIterator

class VertexPairs
{
    public:
        VertexPairs( Vertices   V ) : V_( V ),  begin_( 0 ),            end_( V.size() )    {}

        VertexPairsIterator begin    ()  const {    return VertexPairsIterator( &V_, begin_, end_ ); }
        VertexPairsIterator end      ()  const {    return VertexPairsIterator( &V_, end_,   end_ ); }

    private:
        const Vertices  V_;
        int             begin_,
                        end_;

};  // VertexPairs

} // namespace chordalg

#endif // VERTEX_UTILITIES_H
