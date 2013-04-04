#ifndef ELIMINATION_ORDERING_H
#define ELIMINATION_ORDERING_H

#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <iterator>

#include "graph.h"
#include "intersection_graph.h"

namespace chordalg {

class EliminationOrdering
{
    public:
        EliminationOrdering( Graph& G );
        EliminationOrdering( EliminationOrdering& eo );
        virtual ~EliminationOrdering();

        int fill_size(){ return fill_size_; }
        VertexContainer Ordering(){ return alpha_; }

        inline bool IsFillEdge( Vertex u, Vertex w ) { return fill_edge_count_[ u ][ w ] > 0; }
        bool IsOrderConsistent();
        void PrettyPrint();

        void MoveAndPush( int, int );
        void Swap( int, int );

    protected:

        virtual void Init();

        template< class DerivedFromEO >
        void ForEachFillPair( Vertex, void ( DerivedFromEO::*body )( Vertex, Vertex ) );
        virtual VertexContainer MonotoneAdjacencySet( Vertex );

        void Fill( int i ){ Refill( i, i ); return; }

        void RefillBody( Vertex, Vertex );
        void Refill( int, int );
        void UnfillBody( Vertex, Vertex );
        void Unfill( int, int );

        void AddEdge( Vertex, Vertex );
        void RemoveEdge( Vertex, Vertex );
        void SwapPair( int, int );

        Graph& G_;
        VertexContainer alpha_;             // alpha[i] = ith vertex eliminated
        std::vector< int > alpha_inverse_;  // alpha_inverse[v] = elimination # of v

        int fill_size_;
        std::vector< std::vector< int > > fill_edge_count_; // # of vertices
        std::vector< std::map< Vertex, bool > > fill_neighbors_;

}; // class EliminationOrdering

// Derived classes require templating due to function pointer
template< class DerivedFromEO >
void EliminationOrdering::ForEachFillPair( Vertex v, void ( DerivedFromEO::*body )( Vertex, Vertex ) )
{
    VertexContainer N_alpha = MonotoneAdjacencySet( v );
    DerivedFromEO* derived_this = static_cast< DerivedFromEO* >( this );

    for( VertexIterator u_itr = N_alpha.begin(); u_itr != N_alpha.end(); ++u_itr )
    {
        VertexIterator w_itr = u_itr;
        for( ++w_itr; w_itr != N_alpha.end() ; ++w_itr )
        {
            Vertex u = *u_itr,  w = *w_itr;
            if( !G_.HasEdge( u, w ) )
                ( derived_this->*body )( u, w );
        }
    }
    return;
}

} // namespace chordalg

#endif // ELIMINATION_ORDERING_H
