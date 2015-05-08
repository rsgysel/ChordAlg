/*
 *  mcs_m.h - computes a minimal triangulation
 */

#ifndef CHORDALGSRC_MCS_M_H_
#define CHORDALGSRC_MCS_M_H_

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

void MCSmPlus(
    const Graph&,
    EliminationOrder*,
    FillEdges*,
    VertexList* minsep_generators = nullptr);

}  // namespace chordalg

#endif  // CHORDALGSRC_MCS_M_H_
