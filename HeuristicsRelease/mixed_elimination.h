/*
 *  mixed_elimination.h
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
 *  This class uses both classic and lb-elimination strategies to compute
 *  a triangulation of a graph. In particular, it will score vertices
 *  classicly (because lb-elimination scoring requires a graph search for
 *  each vertex), and eliminated them using lb-elimination (results in a
 *  minimal triangulation). At each step, the classic score of a vertex is
 *  an upper bound on the cost of lb-eliminating that vertex.
 */

#ifndef MIXED_ELIMINATION_H
#define MIXED_ELIMINATION_H

#include "intersection_graph.h"
#include "classic_elimination.h"
#include "lb_elimination.h"
#include "vertex_utilities.h"

namespace chordalg {

class MixedElimination : public LBElimination
{
    public:
        MixedElimination( ColoredIntersectionGraph&, LBCriterion* );
        virtual ~MixedElimination();

    private:
        void                        Eliminate   ( Vertex );
        std::pair< Weight, Cost >   WeightOf    ( Vertex );

        ColoredIntersectionGraph&   H_;
        SeparatorBlocks             B_;
//        RatioCriterion*             f_;
};  // MixedElimination

}  // namespace chordalg

#endif // MIXED_ELIMINATION_H
