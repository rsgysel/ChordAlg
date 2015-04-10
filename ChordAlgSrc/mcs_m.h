/*
 *  mcs_m - computes a minimal triangulation
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

#ifndef INCLUDE_MCS_M_H_
#define INCLUDE_MCS_M_H_

#include <algorithm>
#include <list>
#include <vector>

#include "graph.h"
#include "vertices.h"

namespace chordalg {

void MCSmPlus(const Graph&, VertexVector&, std::vector< size_t >&, std::vector< VertexList >&, VertexList&);

}  // namespace chordalg

#endif  // INCLUDE_MCS_M_H_
