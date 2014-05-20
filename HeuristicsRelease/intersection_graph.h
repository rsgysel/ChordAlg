/*
 *  intersection_graph.h - an intersection graph data structure
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

#ifndef INTERSECTION_GRAPH_H
#define INTERSECTION_GRAPH_H

#include <list>
#include <vector>

#include "graph.h"
#include "lex_trie.h"
#include "vertex_utilities.h"

namespace chordalg {

class ColoredIntersectionGraph : public Graph
{
    public:
        ColoredIntersectionGraph( MatrixCellIntGraphFR* );
        ColoredIntersectionGraph( ColoredIntersectionGraph&, Vertices );
        virtual ~ColoredIntersectionGraph();

        bool                IsMonochromatic ( Vertex, Vertex    );
        int                 CommonColorCount( Vertex, Vertex    );
        const Multicolor&   vertex_color    ( Vertex v          ){ return vertex_colors_[ v ]; }

        void PrettyPrintSubsets();

        const LexTrie*  subset_family()     const { return subset_family_;  }
        const Subset&   subset( Vertex v )  const { return subsets_[ v ];   }
        const std::string& taxon_name( Element e ) const { return taxon_name_[e]; }

    private:
        std::vector< Subset >       subsets_;
        std::vector< Multicolor >   vertex_colors_;
        LexTrie*                    subset_family_;
        std::vector< std::string >  taxon_name_;

        std::vector< std::string >  DefaultTaxonNames   ( int n                                 );
        std::vector< Subset >       InduceSubsets       ( ColoredIntersectionGraph&, Vertices   );
        std::vector< Multicolor >   InduceVertexColors  ( ColoredIntersectionGraph&, Vertices   );
        LexTrie*                    InduceSubsetFamily  ( ColoredIntersectionGraph&             );
}; // ColoredIntersectionGraph

} // namespace chordalg

#endif // INTERSECTION_GRAPH_H
