#ifndef CLASSIC_HEURISTICS_H
#define CLASSIC_HEURISTICS_H

#include <algorithm>

#include "graph_types.h"
#include "elimination_order.h"

namespace chordalg {

class MonochromaticDeficiencyHeuristic : public EliminationOrder
{
    public:
        MonochromaticDeficiencyHeuristic( ColoredIntersectionGraph& H );
        virtual ~MonochromaticDeficiencyHeuristic();

    private:
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        ColoredIntersectionGraph& H_;
}; // class MonochromaticDeficiencyHeuristic

} // namespace chordalg

#endif // CLASSIC_HEURISTICS_H
