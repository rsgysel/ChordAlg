/*
 *  minimal_separator_algorithms.h - algorithms for all or vertex-pair
 *  minimal separator computation
 */

#ifndef INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_
#define INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_

#include <algorithm>
#include <sstream>
#include <string>

#include "graph.h"
#include "separator.h"
#include "separator_graph.h"
#include "vertices.h"

namespace chordalg {

std::string strMinimalSeparators(const MinsepTrie&, const Graph&);

// All minimal separators
MinsepTrie* MinimalSeparators(const Graph& G);

// minimal ab-separators
MinsepTrie* MinimalSeparators(const Graph& G, Vertex a, Vertex b);

}  // namespace chordalg

#endif  // INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_
