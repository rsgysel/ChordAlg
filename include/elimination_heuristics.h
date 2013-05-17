#ifndef ELIMINATION_HEURISTICS_H
#define ELIMINATION_HEURISTICS_H

#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <climits>
#include <iterator>

#include "atom_subgraphs.hpp"
#include "elimination_ordering.h"
#include "intersection_graph.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT UINT_MAX;

// Picks vertex that adds fewest monochromatic fill edges ( i.e. compatible with two-state CR, not general CR )
class MonochromaticFillPairHeuristic : public EliminationOrdering
{
    typedef unsigned int Weight;

    public:
        MonochromaticFillPairHeuristic( ColoredIntersectionGraph& H );
        ~MonochromaticFillPairHeuristic();

        Weight fill_weight() const { return fill_weight_; }

    private:
        ColoredIntersectionGraph& H_;

        void Init();

        Weight monochromatic_fill_pairs_count_, fill_weight_;
        void MinBody( Vertex, Vertex );
        std::pair< Weight, Vertex > Min( std::map< Vertex, bool >&, int );


}; // class MonochromaticFillPairHeuristic

} // namespace chordalg

#endif // ELIMINATION_HEURISTICS_H
