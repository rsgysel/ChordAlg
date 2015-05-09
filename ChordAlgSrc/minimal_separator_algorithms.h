/*
 *  minimal_separator_algorithms.h - algorithms for all or vertex-pair
 *  minimal separator computation
 */

#ifndef CHORDALGSRC_MINIMAL_SEPARATOR_ALGORITHMS_H_
#define CHORDALGSRC_MINIMAL_SEPARATOR_ALGORITHMS_H_

#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator_graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

std::string strMinimalSeparators(const MinsepTrie*, const Graph*);

// All minimal separators
MinsepTrie* MinimalSeparators(const Graph* G);

// minimal ab-separators
MinsepTrie* MinimalSeparators(const Graph* G, Vertex a, Vertex b);

}  // namespace chordalg

#endif  // CHORDALGSRC_MINIMAL_SEPARATOR_ALGORITHMS_H_
