/*
 *  separator.h - a data structure representing a vertex separator, and
 *  its connected components and their neighborhoods
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

#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <algorithm>
#include <set>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "vertex_utilities.h"

namespace chordalg {

typedef int                         ConnectedComponentID;
typedef std::vector< VertexSet >    FillSet;

// Calculates the connected components of G - S for a graph G and vertex set S
// Intended for numerous computations on the same graph.
//
class SeparatorComponents
{
    public:
        explicit SeparatorComponents( Graph const& );
        ~SeparatorComponents() {};

        Vertices        GetNeighborhood     ( Vertex, FillSet& );
        virtual void    Separate            ( Vertices const &, FillSet& fill = *( new FillSet() ) );
        Vertices        ConnectedComponent  ( Vertex ) const;

        int size() const
            { return size_; }

        ConnectedComponentID ComponentId( Vertex v ) const
            { return connected_component_[ v ]; }

        ConnectedComponentID kInSeparator() const   { return -2; }
        ConnectedComponentID kUnsearched()  const   { return -1; }
        ConnectedComponentID kRemoved()     const   { return -3; }

        inline bool IsInSeparator( Vertex u ) const
            { return connected_component_[ u ] == kInSeparator(); }

        inline bool AreConnected( Vertex u, Vertex v ) const
            { return ( IsInSeparator( u ) || IsInSeparator( v ) ) ? false : ( connected_component_[ u ] == connected_component_[ v ] ); }

        // AreSeparated is not the complement of AreConnected, because vertices in S_ are neither connected or separated
        inline bool AreSeparated( Vertex u, Vertex v ) const
            { return ( IsInSeparator( u ) || IsInSeparator( v ) ) ? false : ( connected_component_[ u ] != connected_component_[ v ] ); }

        virtual void PrettyPrint();

    protected:
        void FindComponents( FillSet& );

        Graph const&            G_;
        Vertices                S_;

        ComputationBuffer       connected_component_;
        ComputationBuffer       search_queue_;

        int                     size_;                  // # of connected components

        inline bool IsUnsearched( Vertex u ) const
            { return connected_component_[ u ] == kUnsearched(); }

        // Disable default constructor, copy constructor, assignment
        SeparatorComponents();
        SeparatorComponents(const SeparatorComponents&);
        void operator=(const SeparatorComponents&);
};

class SeparatorBlocks : public SeparatorComponents
{
    public:
        SeparatorBlocks( Graph const& );
        ~SeparatorBlocks() {};

        void Separate( Vertices const &, FillSet& fill = *( new FillSet() ) );

        std::vector< Vertices >::const_iterator begin()
            { return neighborhoods_.begin(); }

        std::vector< Vertices >::const_iterator end()
            { return neighborhoods_.end(); }

        void PrettyPrint();

    private:
        void FindNeighborhoods( FillSet& );

        std::vector< Vertices >     neighborhoods_;
        ComputationBuffer           last_separator_vertex_seen_;

        // Disable default constructor, copy constructor, assignment
        SeparatorBlocks();
        SeparatorBlocks(const SeparatorBlocks&);
        void operator=(const SeparatorBlocks&);
};

}   // namespace chordalg

#endif // SEPARATOR_H
