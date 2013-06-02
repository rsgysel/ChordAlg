#ifndef ELIMINATION_ORDER_H
#define ELIMINATION_ORDER_H

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <set>
#include <vector>

#include "chordalg_types.h"
#include "graph_types.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

struct EliminationCriterion
{
    virtual Weight Calculate    ( Weight deficiency )                   = 0;
    virtual Weight Calculate    ( Weight deficiency, Weight separated ) = 0;
}; // struct EliminationCriterion

class EliminationOrder
{
    public:
        EliminationOrder( Graph& );
        virtual ~EliminationOrder();

        void    PrettyPrint() const;

        Weight                          fill_cost()     const   { return fill_cost_;    }
        int                             fill_count()    const   { return fill_count_;   }
        const std::vector< int >&       tie_count()     const   { return tie_count_;    }

    protected:
        virtual void Init();

        // Main Methods
        void                Elimination     ( );
        VertexCost          ArgMin          ( );
        virtual VertexCost  TieBreak        ( );        // base method breaks ties uniformly
        Vertices            MonotoneNbhd    ( Vertex );

        // Helpers
        void AddEdge        ( VertexPair    );
        bool IsEdge         ( VertexPair    );
        bool IsFillEdge     ( VertexPair    );
        bool IsRemoved      ( Vertex        );
        void Saturate       ( Vertices      );

        virtual void                        Eliminate   ( Vertex ) = 0;
        virtual std::pair< Weight, Cost >   WeightOf    ( Vertex ) = 0;

        Graph&                          G_;

        Vertices                        alpha_;             // alpha[i] = ith vertex eliminated
        std::vector < int           >   alpha_inverse_;     // alpha_inverse[v] = elimination # of v

        Weight                          fill_cost_;         //
        int                             fill_count_;        // # of fill edges added (monochromatic or not)
        std::vector < VertexSet     >   fill_neighbors_;
        VertexSet                       remaining_vertices_;

        std::vector < VertexCost    >   ties_;
        std::vector < int           >   tie_count_;
}; // EliminationOrder

} // namespace chordalg

#endif // ELIMINATION_ORDER_H
