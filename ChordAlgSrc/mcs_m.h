/*
 *  mcs_m.h - computes a minimal triangulation
 */

#ifndef INCLUDE_MCS_M_H_
#define INCLUDE_MCS_M_H_

#include <algorithm>
#include <list>
#include <vector>

#include "elimination_order.h"
#include "graph.h"
#include "vertices.h"

namespace chordalg {

void MCSmPlus(const Graph&, EliminationOrder&, std::vector< VertexList >&, VertexList&);

}  // namespace chordalg

#endif  // INCLUDE_MCS_M_H_
