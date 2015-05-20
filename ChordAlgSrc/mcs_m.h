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

namespace MCSmPlus {

void Run(
    const Graph&,
    EliminationOrder*,
    FillEdges*,
    VertexList* minsep_generators = nullptr);

}  // namespace MCSmPlus

}  // namespace chordalg

#endif  // CHORDALGSRC_MCS_M_H_
