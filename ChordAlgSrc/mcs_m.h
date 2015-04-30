/*
 *  mcs_m.h - computes a minimal triangulation
 */

#ifndef CHORDALGSRC_MCS_M_H_
#define CHORDALGSRC_MCS_M_H_

#include <vector>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

void MCSmPlus(
    const Graph&,
    EliminationOrder&,
    std::vector< VertexList >&,
    VertexList&);

}  // namespace chordalg

#endif  // CHORDALGSRC_MCS_M_H_
