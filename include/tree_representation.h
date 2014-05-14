#ifndef TREE_REPRESENTATION_H
#define TREE_REPRESENTATION_H

#include "graph.h"

namespace chordalg {

class TreeRepresentation
{
    public:
        TreeRepresentation          ( AdjacencyLists* E,  VertexNames K, Graph& G   );
        virtual ~TreeRepresentation (                                               );

        void            PrettyPrint()   const   { T_.PrettyPrint(); }

        const Graph&    G()             const   { return G_;        }
        const Graph&    T()             const   { return T_;        }
    protected:
        Graph&  G_;     // underlying chordal graph
        Graph   T_;     // topology
}; // TreeRepresentation

class CliqueTree : public TreeRepresentation
{
    public:
        CliqueTree  ( AdjacencyLists* E,  VertexNames K, Graph& G   );
        ~CliqueTree (                                               );
}; // CliqueTree

}; // namespace chordalg

#endif // TREE_REPRESENTATION_H
