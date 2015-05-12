#ifndef CHORDALGSRC_LB_TRIANG_H_
#define CHORDALGSRC_LB_TRIANG_H_

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"

namespace chordalg {

Triangulation* LBTriang(const Graph*, const EliminationOrder* = nullptr);

}  // namespace chordalg

#endif  // CHORDALGSRC_LB_TRIANG_H_
