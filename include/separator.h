#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <algorithm>
#include <set>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "utilities.h"

namespace chordalg {

typedef int                                 ConnectedComponentID;
typedef std::vector< std::set< Vertex > >   FillSet;

// Calculates the connected components of G - S for a graph G and vertex set S
// Intended for numerous computations on the same graph.
//
class SeparatorComponents
{
    public:
        explicit SeparatorComponents( Graph const& );
        ~SeparatorComponents() {};

        const VertexContainer& GetNeighborhood( Vertex, FillSet& );
        virtual void Separate( VertexContainer const &, FillSet& fill = *( new FillSet() ) );
        VertexContainer ConnectedComponent( Vertex ) const;

        int size() const
            { return size_; }

        ConnectedComponentID ComponentId( Vertex v ) const
            { return connected_component_[ v ]; }

        ConnectedComponentID kInSeparator() const   { return -2; }
        ConnectedComponentID kUnsearched() const    { return -1; }
        ConnectedComponentID kRemoved() const       { return -3; }

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

        Graph const& G_;
        ComputationBuffer S_;

        ComputationBuffer connected_component_;
        ComputationBuffer search_queue_;

        int size_;                                                     // # of connected components


        inline bool IsUnsearched( Vertex u ) const
            { return connected_component_[ u ] == kUnsearched(); }

        DISALLOW_DEFAULT_CONSTRUCTOR( SeparatorComponents );
        DISALLOW_COPY_AND_ASSIGN( SeparatorComponents );
};

class SeparatorBlocks : public SeparatorComponents
{
    public:
        SeparatorBlocks( Graph const& );
        ~SeparatorBlocks() {};

        void Separate( VertexContainer const &, FillSet& fill = *( new FillSet() ) );

        ComputationBufferSet::const_iterator begin()
            { return neighborhoods_.begin(); }

        ComputationBufferSet::const_iterator end()
            { return neighborhoods_.end(); }

        void PrettyPrint();

    private:
        void FindNeighborhoods( FillSet& );

        ComputationBufferSet neighborhoods_;
        ComputationBuffer last_separator_vertex_seen_;

        DISALLOW_DEFAULT_CONSTRUCTOR( SeparatorBlocks );
        DISALLOW_COPY_AND_ASSIGN( SeparatorBlocks );
};

}   // namespace chordalg

#endif // SEPARATOR_H
