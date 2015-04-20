/*
 *  atoms.h - atom subgraphs
 *  Copyright (C) 2013 Rob Gysel
 */

#ifndef INCLUDE_ATOMS_H_
#define INCLUDE_ATOMS_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <vector>

#include "elimination_order.h"
#include "graph.h"
#include "lex_trie.h"
#include "mcs_m.h"
#include "minimal_separator_algorithms.h"
#include "separator.h"
#include "vertices.h"

namespace chordalg {

class Atoms {
 public:
    Atoms() = delete;
    Atoms(const Atoms&) = delete;
    Atoms& operator=(const Atoms&) = delete;

    explicit Atoms(const Graph* G);
    ~Atoms();

    const LexTrie& clique_minimal_separators() const {
        return clique_minimal_separators_;
    }
    int size() {
        return atom_subgraphs_.size();
    }

    typedef typename std::vector< InducedSubgraph* >::const_iterator AtomIterator;
    AtomIterator begin() {
        return atom_subgraphs_.begin();
    }
    AtomIterator end() {
        return atom_subgraphs_.end();
    }

    void ComputeAtoms();

 private:
    std::vector< InducedSubgraph* > atom_subgraphs_;
    const Graph* const G_;
    LexTrie clique_minimal_separators_;
};  // Atoms

}  // namespace chordalg

#endif  // INCLUDE_ATOMS_H_
