#ifndef MIXED_ELIMINATION_H
#define MIXED_ELIMINATION_H

#include "lb_elimination.h"

namespace chordalg {

class MixedElimination : public LBElimination
{
    public:
        MixedElimination( ColoredIntersectionGraph&, RatioCriterion* );
        virtual ~MixedElimination();

    private:
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        ColoredIntersectionGraph&   H_;
        SeparatorBlocks             B_;
        RatioCriterion*           f_;
};  // MixedElimination

}  // namespace chordalg

#endif // MIXED_ELIMINATION_H
