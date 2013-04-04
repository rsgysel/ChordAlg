#ifndef CHORDALG_TYPES_H
#define CHORDALG_TYPES_H

#include <list>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

// graph typedefs
typedef int Vertex;
typedef std::pair< Vertex, Vertex > VertexPair;
typedef std::vector< Vertex > VertexContainer;
typedef VertexContainer::const_iterator VertexIterator;

typedef VertexContainer Nbhd;
typedef std::vector< Nbhd > AdjacencyLists;

typedef std::string VertexName;
typedef std::vector< std::string > VertexNameContainer;

// other typedefs
typedef std::vector< int > ComputationBuffer;       // buffers re-used in numerous computations
            // Requires back(), clear(), pop_back(), push_back(), resize(), and ranged-for.
typedef std::vector< ComputationBuffer > ComputationBufferSet;

//class Graph;
//typedef Graph AtomSubgraph;
//typedef std::vector< AtomSubgraph* > AtomList;
//typedef AtomList::const_iterator AtomIterator;

// intersection graph typedefs
typedef int Element;
typedef std::vector< Element > Subset;

typedef int Color;
typedef std::list< Color > Multicolor;

} // namespace chordalg

#endif // CHORDALG_TYPES_H
