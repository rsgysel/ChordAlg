/*
 *  minimal_separator_algorithms.h - algorithms for all or vertex-pair
 *  minimal separator computation
 */

#ifndef CHORDALGSRC_MINIMAL_SEPARATORS_H_
#define CHORDALGSRC_MINIMAL_SEPARATORS_H_

#include <stack>
#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef LexTrie                     MinsepTrie;
typedef std::stack< Vertices >      MinsepStack;
typedef std::vector< Vertices >     MinsepVector;

MinsepVector* MinsepTrieToVector(const MinsepTrie&);

namespace MinimalSeparators {

std::string str(const MinsepTrie&, const Graph&);

// All minimal separators
MinsepTrie* Generate(const Graph& G);

// minimal ab-separators
MinsepTrie* Generate(const Graph& G, Vertex a, Vertex b);

}  // namespace MinimalSeparators

}  // namespace chordalg

#endif  // CHORDALGSRC_MINIMAL_SEPARATORS_H_
