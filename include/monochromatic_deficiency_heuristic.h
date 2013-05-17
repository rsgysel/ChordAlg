#ifndef MONOCHROMATIC_DEFICIENCY_HEURISTIC_H
#define MONOCHROMATIC_DEFICIENCY_HEURISTIC_H

#include <algorithm>

#include "elimination_order.h"
#include "intersection_graph.h"

namespace chordalg {

class MonochromaticDeficiencyHeuristic : EliminationOrder
{
    public:
        MonochromaticDeficiencyHeuristic( ColoredIntersectionGraph& H );
        virtual ~MonochromaticDeficiencyHeuristic();

    private:
        void    Eliminate   ( Vertex );
        Weight  WeightOf    ( Vertex );

        ColoredIntersectionGraph& H_;
};

} // namespace chordalg

#endif // MONOCHROMATIC_DEFICIENCY_HEURISTIC_H
