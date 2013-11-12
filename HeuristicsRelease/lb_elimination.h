#ifndef SEPARATION_HEURISTICS_H
#define LB_ELIMINATION_H

#include <algorithm>

#include "elimination_order.h"
#include "intersection_graph.h"
#include "separator.h"

namespace chordalg {

struct LBCriterion : public EliminationCriterion
{
    public:
        virtual Weight Calculate    ( Weight deficiency, Weight separated ) { return 0; }
    private:
        virtual Weight Calculate    ( Weight deficiency )                   { return 0; }
}; // DeficiencyCriterion

struct RatioCriterion : public LBCriterion
{
    Weight Calculate    ( Weight deficiency, Weight separated ) { return deficiency / (1 + separated); }
}; // RatioCriterion

struct WSumCriterion : public LBCriterion
{
    public:
        WSumCriterion(                      )   : d_( 1 ), s_( 1 )  {};
        WSumCriterion( Weight d, Weight s   )   : d_( d ), s_( s )  {};

        Weight Calculate ( Weight deficiency, Weight separated ) { return d_*deficiency - s_*separated; }
    private:
        Weight d_, s_;
}; // WSumCriterion

class LBElimination : public EliminationOrder
{
    public:
        LBElimination( ColoredIntersectionGraph&, LBCriterion* );
        virtual ~LBElimination();

    protected:
        void                        Init        ( );
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        virtual std::pair< Weight, Cost >   ObjectiveFunction( Weight, Weight );

        ColoredIntersectionGraph&   H_;
        SeparatorBlocks             B_;
        LBCriterion*                f_;

        std::map< VertexPair, Weight >  unseparated_monochromatic_pairs_;
}; // class LBElimination

} // namespace chordalg

#endif // LB_ELIMINATION_H
