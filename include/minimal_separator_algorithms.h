#ifndef MINIMAL_SEPARATOR_ALGORITHMS_H_INCLUDED
#define MINIMAL_SEPARATOR_ALGORITHMS_H_INCLUDED


#include <algorithm>
#include <vector>

#include "separator.h"
#include "graph.h"
#include "lex_trie.h"

namespace chordalg {

LexTrie& BerryBordatCogis(Graph G);

} // namespace chordalg

#endif // MINIMAL_SEPARATOR_ALGORITHMS_H_INCLUDED
