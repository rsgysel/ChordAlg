/*
 *  classic_elimination.h - computes a triangulation via elimination
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  This algorithm implements the classical approach to triangulating
 *  a graph using elimination orderings. See Rose "Triangulated graphs and
 *  the elimination process"
 */

#ifndef CLASSIC_ELIMINATION_H
#define CLASSIC_ELIMINATION_H

#include <algorithm>

#include "elimination_order.h"
#include "intersection_graph.h"
#include "vertex_utilities.h"

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
