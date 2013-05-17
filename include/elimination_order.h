#ifndef ELIMINATION_ORDER_H
#define ELIMINATION_ORDER_H

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <set>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"

namespace chordalg {

typedef double Weight;  // argument to minimize
typedef double Cost;    // price for fill edge
typedef std::pair< Vertex, Cost > VertexCost;

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

class EliminationOrder
{
    public:
        EliminationOrder( Graph& G );
        virtual ~EliminationOrder();

        Weight fill_cost() { return fill_cost_; }

    protected:
        virtual void Init();

        // Main Methods
        void                Elimination ( );
        VertexCost          ArgMin      ( );
        virtual VertexCost  TieBreak    ( );        // base method breaks ties uniformly

        // Helpers
        void AddEdge        ( VertexPair        );
        bool IsEdge         ( VertexPair        );
        bool IsFillEdge     ( VertexPair        );
        bool IsRemoved      ( Vertex            );
        void Saturate       ( VertexContainer   );

        VertexContainer MonotoneNbhd( Vertex );

        virtual void                        Eliminate   ( Vertex ) = 0;
        virtual std::pair< Weight, Cost >   WeightOf    ( Vertex ) = 0;


        Graph&                                  G_;

        VertexContainer                         alpha_;             // alpha[i] = ith vertex eliminated
        std::vector < int                   >   alpha_inverse_;     // alpha_inverse[v] = elimination # of v

        int                                     fill_count_;
        Weight                                  fill_cost_;
        std::vector < std::set< Vertex >    >   fill_neighbors_;
        std::set    < Vertex                >   remaining_vertices_;

        std::vector < VertexCost            >   ties_;
        std::vector < int                   >   tie_count_;
}; // EliminationOrder

} // namespace chordalg

#endif // ELIMINATION_ORDER_H
