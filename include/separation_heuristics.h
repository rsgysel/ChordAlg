#ifndef SEPARATION_HEURISTICS_H
#define SEPARATION_HEURISTICS_H

#include <algorithm>

#include "elimination_order.h"
#include "intersection_graph.h"
#include "separator.h"

namespace chordalg {

class MonochromaticPDRS : public EliminationOrder
{
    public:
        MonochromaticPDRS( ColoredIntersectionGraph& H );
        virtual ~MonochromaticPDRS();

    private:
        void                        Init        ( );
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        virtual std::pair< Weight, Cost >   ObjectiveFunction( Weight, Weight );

        ColoredIntersectionGraph&   H_;
        SeparatorBlocks             B_;

        std::map< VertexPair, Weight >  unseparated_monochromatic_pairs_;
}; // class MonochromaticPDRS

} // namespace chordalg

#endif // SEPARATION_HEURISTICS_H
