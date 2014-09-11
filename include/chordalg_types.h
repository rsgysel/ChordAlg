/*
 *  chordalg_types.h - just some typedefs
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

#ifndef INCLUDE_CHORDALG_TYPES_H_
#define INCLUDE_CHORDALG_TYPES_H_

#include <list>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

// buffers re-used in numerous computations
typedef std::vector< int > ComputationBuffer;

// Requires back(), clear(), pop_back(), push_back(), resize(), and ranged-for
typedef std::vector< ComputationBuffer > ComputationBufferSet;

// intersection graph typedefs
typedef int Element;
typedef std::vector< Element > Subset;

typedef int Color;
typedef std::list< Color > Multicolor;

}  // namespace chordalg

#endif  // INCLUDE_CHORDALG_TYPES_H_
