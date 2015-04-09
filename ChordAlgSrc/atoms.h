/*
 *  atoms.h - atom subgraphs
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_ATOMS_H_
#define INCLUDE_ATOMS_H_

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "lex_trie.h"
#include "mcs_m.h"
#include "separator.h"
#include "vertex_utilities.h"

namespace chordalg {

class Atoms {
 public:
    Atoms() = delete;
    Atoms(const Atoms&) = delete;
    Atoms& operator=(const Atoms&) = delete;

    explicit Atoms(const Graph* G);
    ~Atoms();

    int size() {
        return atom_subgraphs_.size();
    }

    typedef typename std::vector< Graph* >::const_iterator AtomIterator;
    AtomIterator begin() {
        return atom_subgraphs_.begin();
    }
    AtomIterator end() {
        return atom_subgraphs_.end();
    }

 private:
    void ComputeAtoms();

    std::vector< Graph* > atom_subgraphs_;

    const Graph* const G_;
    VertexVector alpha_;  // peo. alpha[i] is i^th vertex eliminated
    std::vector< size_t > alpha_inverse_;
    std::vector< VertexList > F_;  // as in paper; minimal fill
    VertexList minsep_generators_;  // 'X' in paper
};  // Atoms

}  // namespace chordalg

#endif  // INCLUDE_ATOMS_H_
