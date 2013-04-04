#ifndef LB_TRIANG_HEURISTIC_H
#define LB_TRIANG_HEURISTIC_H

#include <algorithm>
#include <cfloat>
#include <climits>
#include <map>
#include <set>

#include "chordalg_types.h"
#include "elimination_ordering.h"
#include "intersection_graph.h"
#include "separator.h"

namespace chordalg {

typedef unsigned int Weight;
#define MAX_WEIGHT UINT_MAX;

class LBTriangHeuristic : public EliminationOrdering
{
    public:
        LBTriangHeuristic( ColoredIntersectionGraph& G );
        virtual ~LBTriangHeuristic();


    protected:
        ColoredIntersectionGraph& H_;
        SeparatorBlocks B_;

        std::vector< Weight > fill_of_ith_vertex_, separated_of_ith_vertex_;

        void Init();

        void Saturate( Vertex, Vertex );
        void FindSaturatingSets( Vertex );

        template< class DerivedFromEO >
        void ForEachSaturatingSet( Vertex, void ( DerivedFromEO::*body )( Vertex, Vertex ) );

        void CountSeparatedMonochromaticPair( Vertex );
        void UpdateMonochromaticPairs( Vertex );

        Weight fill_cost_;
        double fill_weight_, separated_weight_;
        std::map< std::pair< Vertex, Vertex >, Weight > monochromatic_pairs_; // RF: unseparated_monochromatic_pairs_
        std::set< std::pair< Vertex, Vertex > > fill_from_v_;

        void MinBody( Vertex, Vertex );
        std::pair< std::pair< Weight, double >, Vertex > Min( std::map< Vertex, bool >&, int );


};

// Derived classes require templating due to function pointer
template< class DerivedFromEO >
void LBTriangHeuristic::ForEachSaturatingSet( Vertex v, void ( DerivedFromEO::*body )( Vertex, Vertex ) )
{

    for( VertexContainer NC : B_ )
    {
        // ForEachFillEdge( NC, funcbody )
        for( VertexIterator u_itr = NC.begin(); u_itr != NC.end(); ++u_itr )
        {
            VertexIterator w_itr = u_itr;
            for( ++w_itr; w_itr != NC.end() ; ++w_itr )
            {
                Vertex u = *u_itr,  w = *w_itr;
                if( !G_.HasEdge( u, w ) )
                    ( this->*body )( u, w );
            }
        }
    }

    return;
}

} // namespace chordalg

#endif // LB_TRIANG_HEURISTIC_H
