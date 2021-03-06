#ifndef CHORDALGSRC_ELIMINATION_H_
#define CHORDALGSRC_ELIMINATION_H_

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"

namespace chordalg {

namespace Elimination {

AdjacencyLists* FillIn(const Graph&, const EliminationOrder&);

bool ZeroFill(const Graph&, const EliminationOrder&);

EliminationOrder* ConnectedOrder(const Graph&);

}  // namespace Elimination

}  // namespace chordalg

#endif  // CHORDALGSRC_ELIMINATION_H_
