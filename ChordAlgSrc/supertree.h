/*
 * supertree.h - converts a clique tree of a triangulation H of a 
 * cell intersection graph into a newick string for a supertree
 * that displays the un-broken characters of H
 */

#ifndef CHORDALGSRC_SUPERTREE_H_
#define CHORDALGSRC_SUPERTREE_H_

#include <string>

#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/tree_representation.h"

namespace chordalg {

namespace SuperTree {

std::string NewickStr(const CliqueTree&, const CellIntersectionGraph&);

}  // namespace SuperTree

}  // namespace chordalg

#endif  // CHORDALGSRC_SUPERTREE_H_
