/*
 *  minimal_separator_algorithms.h
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

#ifndef INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_
#define INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_

#include <algorithm>
#include <vector>

#include "graph.h"
#include "lex_trie.h"
#include "separator.h"
#include "vertex_utilities.h"

namespace chordalg {

void PrettyPrintMinimalSeparators(const LexTrie& minseps, const Graph& G);

// All minimal separators
LexTrie* MinimalSeparators(const Graph& G);

// minimal ab-separators
LexTrie* MinimalSeparators(const Graph& G, Vertex a, Vertex b);

}  // namespace chordalg

#endif  // INCLUDE_MINIMAL_SEPARATOR_ALGORITHMS_H_
