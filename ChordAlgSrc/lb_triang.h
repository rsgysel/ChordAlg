#ifndef CHORDALGSRC_LB_TRIANG_H_
#define CHORDALGSRC_LB_TRIANG_H_

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"

namespace chordalg {

namespace LBTriang {

Triangulation* Run(const Graph*, const EliminationOrder* = nullptr);

}  // namespace LBTriang

}  // namespace chordalg

#endif  // CHORDALGSRC_LB_TRIANG_H_
