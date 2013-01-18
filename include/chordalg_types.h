#ifndef CHORDALG_TYPES_H_INCLUDED
#define CHORDALG_TYPES_H_INCLUDED

#include <string>
#include <vector>

// delete me
#include <iterator>

namespace chordalg {

// graph typedefs
typedef int Vertex;
typedef std::vector< Vertex > VertexContainer;
typedef VertexContainer::const_iterator VertexIterator;

typedef VertexContainer Nbhd;
typedef std::vector< Nbhd > AdjacencyLists;

typedef std::string VertexName;
typedef std::vector< VertexName > VertexNameContainer;

// other typedefs
typedef std::vector< int > ComputationBuffer;       // buffers that will be re-used over numerous computations.
            // Requires back(), clear(), pop_back(), push_back(), resize(), and ranged-for.
typedef std::vector< ComputationBuffer> ComputationBufferSet;

} // namespace chordalg

#endif // CHORDALG_TYPES_H_INCLUDED
