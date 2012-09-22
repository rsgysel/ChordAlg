#ifndef CHORDALG_TYPES_H_INCLUDED
#define CHORDALG_TYPES_H_INCLUDED

#include <string>
#include <vector>

namespace chordalg {

// graph typedefs
typedef int Vertex;
typedef std::vector< Vertex > VertexContainer;
typedef VertexContainer::const_iterator VertexIterator;

typedef VertexContainer Nbhd;
typedef std::vector< Nbhd > AdjacencyLists;

typedef std::string VertexName;
typedef std::vector< VertexName > VertexNameContainer;

typedef int ConnectedComponentOf;

} // namespace chordalg

#endif // CHORDALG_TYPES_H_INCLUDED
