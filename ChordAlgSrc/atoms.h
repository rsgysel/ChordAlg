/*
 *  atoms.h - atom subgraphs
 */

#ifndef CHORDALGSRC_ATOMS_H_
#define CHORDALGSRC_ATOMS_H_

#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lex_trie.h"

namespace chordalg {

typedef typename std::vector< InducedSubgraph* >::const_iterator AtomIterator;

class Atoms {
 public:
    Atoms() = delete;
    Atoms(const Atoms&) = delete;
    Atoms& operator=(const Atoms&) = delete;

    ~Atoms();

    static Atoms* New(const Graph*);
    const LexTrie& clique_minimal_separators() const;
    int size();
    AtomIterator begin();
    AtomIterator end();

 private:
    explicit Atoms(const Graph*);
    void ComputeAtoms();

    std::vector< InducedSubgraph* > atom_subgraphs_;
    const Graph* const G_;
    LexTrie clique_minimal_separators_;
};  // Atoms

}  // namespace chordalg

#endif  // CHORDALGSRC_ATOMS_H_
