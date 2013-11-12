#ifndef CHORDALG_TYPES_H
#define CHORDALG_TYPES_H

#include <list>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

// other typedefs
typedef std::vector< int > ComputationBuffer;       // buffers re-used in numerous computations
            // Requires back(), clear(), pop_back(), push_back(), resize(), and ranged-for.
typedef std::vector< ComputationBuffer > ComputationBufferSet;

// intersection graph typedefs
typedef int Element;
typedef std::vector< Element > Subset;

typedef int Color;
typedef std::list< Color > Multicolor;

} // namespace chordalg

#endif // CHORDALG_TYPES_H
