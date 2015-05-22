#ifndef CHORDALGSRC_POTENTIAL_MAXCLIQUES_H_
#define CHORDALGSRC_POTENTIAL_MAXCLIQUES_H_

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef LexTrie PMCTrie;

namespace PotentialMaxcliques {

// Generates the potential maxcliques of a graph G
// WARNING: assumes G is connected.
// Either compute connected InducedSubgraphs
// or compute AtomSubgraphs first.
PMCTrie* Generate(const Graph&);

PMCTrie* OneMoreVertex(
    const Graph&,
    Vertex,
    const Graph&,
    const PMCTrie&,
    const MinsepTrie&,
    const MinsepTrie&);

bool IsPMC(const Graph&, const Vertices&);

}  // namespace PotentialMaxcliques

}  // namespace chordalg

#endif  // CHORDALGSRC_POTENTIAL_MAXCLIQUES_H_
