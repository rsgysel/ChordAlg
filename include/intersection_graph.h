#ifndef INTERSECTION_GRAPH_H
#define INTERSECTION_GRAPH_H

#include <list>
#include <vector>

#include "graph.h"
#include "lex_trie.h"

namespace chordalg {

class ColoredIntersectionGraph : public Graph
{
    public:
        ColoredIntersectionGraph( MatrixCellIntGraphFR* );
        ColoredIntersectionGraph( ColoredIntersectionGraph&, VertexVector );
        virtual ~ColoredIntersectionGraph();

        bool                IsMonochromatic ( Vertex, Vertex    );
        int                 CommonColorCount( Vertex, Vertex    );
        const Multicolor&   vertex_color    ( Vertex v          ){ return vertex_colors_[ v ]; }

        void PrettyPrintSubsets();

        const LexTrie*  subset_family()     const { return subset_family_;  }
        const Subset&   subset( Vertex v )  const { return subsets_[ v ];   }

    private:

        std::vector< Subset >       subsets_;
        std::vector< Multicolor >   vertex_colors_;
        LexTrie*                    subset_family_;

        std::vector< Subset >       InduceSubsets       ( ColoredIntersectionGraph&, VertexVector   );
        std::vector< Multicolor >   InduceVertexColors  ( ColoredIntersectionGraph&, VertexVector   );
        LexTrie*                    InduceSubsetFamily  ( ColoredIntersectionGraph&                 );
}; // ColoredIntersectionGraph

} // namespace chordalg

#endif // INTERSECTION_GRAPH_H
