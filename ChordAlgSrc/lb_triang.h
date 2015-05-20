/*
 * lb_triang.h - computes a minimal triangulation
 *
 * see Berry et. al.
 * 'A wide-range algorithm for minimal triangulation from an arbitrary ordering'
 * http://www.sciencedirect.com/science/article/pii/S0196677404001142
 */

#ifndef CHORDALGSRC_LB_TRIANG_H_
#define CHORDALGSRC_LB_TRIANG_H_

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"

namespace chordalg {

namespace LBTriang {

Triangulation* Run(const Graph&);

Triangulation* Run(const Graph&, const EliminationOrder&);

}  // namespace LBTriang

}  // namespace chordalg

#endif  // CHORDALGSRC_LB_TRIANG_H_
