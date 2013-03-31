#ifndef INTERSECTION_GRAPH_H
#define INTERSECTION_GRAPH_H

#include <list>
#include <vector>

#include <graph.h>
#include <lex_trie.h>

namespace chordalg {

class ColoredIntersectionGraph : public Graph
{
    public:
        ColoredIntersectionGraph( MatrixCellIntGraphFR* );
        virtual ~ColoredIntersectionGraph();

        void PrettyPrintSubsets();

    private:
        LexTrie* subset_family_;

        std::vector< Subset > subsets_;
        std::vector< Multicolor > vertex_colors_;
};

} // namespace chordalg

#endif // INTERSECTION_GRAPH_H
