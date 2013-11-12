#ifndef CLASSIC_ELIMINATION_H
#define CLASSIC_ELIMINATION_H

#include <algorithm>

#include "graph_types.h"
#include "elimination_order.h"

namespace chordalg {

struct ClassicCriterion : public EliminationCriterion
{
        virtual Weight  Calculate   ( Weight deficiency )                   { return 0; }
    private:
        Weight          Calculate   ( Weight deficiency, Weight separted )  { return 0; }
}; // ClassicCriterion

struct DeficiencyCriterion : public ClassicCriterion
{
    Weight Calculate    ( Weight deficiency ) { return deficiency; }
}; // DeficiencyCriterion

class ClassicElimination : public EliminationOrder
{
    public:
        ClassicElimination( ColoredIntersectionGraph&, ClassicCriterion* );
        virtual ~ClassicElimination();

    private:
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        ColoredIntersectionGraph&   H_;
        ClassicCriterion*           f_;
}; // class ClassicElimination

} // namespace chordalg

#endif // CLASSIC_ELIMINATION_H
