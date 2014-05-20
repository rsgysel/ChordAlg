#ifndef TREE_REPRESENTATION_H
#define TREE_REPRESENTATION_H

#include "graph.h"
#include "intersection_graph.h"

namespace chordalg {

class TreeRepresentation
{
    public:
        TreeRepresentation              ( AdjacencyLists* E, Graph& G, std::vector< Vertices > clique_map   );
        virtual ~TreeRepresentation     (                                                                   );

        void            NewickVisit     (   VertexSet&                          visited,
                                            Vertex                              v,
                                            std::string&                        newick_tree         ) const;
        void            PhyloNewickVisit(   VertexSet&                          visited,
                                            Vertex                              v,
                                            std::string&                        newick_tree,
                                            const ColoredIntersectionGraph&     cig,
                                            std::vector< int >&                 taxon_clique_size   ) const;

        void            NewickPrint     (                                       ) const;
        void            PhyloNewickPrint( const ColoredIntersectionGraph& cig   ) const;
        void            PrettyPrint     (                                       ) const    { T_.PrettyPrint(); }

        const Graph&    G               (                                       ) const    { return G_;        }
        const Graph&    T               (                                       ) const    { return T_;        }
    protected:
        Graph&                  G_;             // underlying chordal graph
        Graph                   T_;             // topology
        std::vector< Vertices > clique_map_;    // maps nodes of T_ to cliques of G_

        std::string SerializeMaxcliqueAsString  ( Vertices K                            ) const;
        VertexNames NamesFromCliqueMap          ( std::vector< Vertices > clique_map    ) const;

}; // TreeRepresentation

class CliqueTree : public TreeRepresentation
{
    public:
        CliqueTree  ( AdjacencyLists* E, Graph& G, std::vector< Vertices > clique_map   );
        ~CliqueTree (                                                                   );
}; // CliqueTree

}; // namespace chordalg

#endif // TREE_REPRESENTATION_H
