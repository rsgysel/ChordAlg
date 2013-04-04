#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <algorithm>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "utilities.h"

namespace chordalg {

typedef int ConnectedComponentID;

// Calculates the connected components of G - S for a graph G and vertex set S
// Intended for numerous computations on the same graph.
//
class SeparatorComponents
{
    public:
        explicit SeparatorComponents( Graph const& );
        ~SeparatorComponents() {};

        virtual void Separate( VertexContainer const & );
        int size() const { return size_; }
        ConnectedComponentID ComponentId( Vertex v ) const { return connected_component_[ v ]; }
        VertexContainer ConnectedComponent( Vertex ) const;

        ConnectedComponentID kInSeparator() const { return -2; }
        ConnectedComponentID kUnsearched() const { return -1; }

        bool IsInSeparator( Vertex );
        bool AreConnected( Vertex, Vertex );
        bool AreSeparated( Vertex, Vertex );

        virtual void PrettyPrint();

    protected:
        void FindComponents();


        Graph const& G_;
        ComputationBuffer S_;

        ComputationBuffer connected_component_;
        ComputationBuffer search_queue_;

        int size_;                                                     // # of connected components

        DISALLOW_DEFAULT_CONSTRUCTOR( SeparatorComponents );
        DISALLOW_COPY_AND_ASSIGN( SeparatorComponents );
};

class SeparatorBlocks : public SeparatorComponents
{
    public:
        SeparatorBlocks( Graph const& );
        ~SeparatorBlocks() {};

        void Separate( VertexContainer const & );
        ComputationBufferSet::const_iterator begin(){ return neighborhoods_.begin(); }
        ComputationBufferSet::const_iterator end(){ return neighborhoods_.end(); }

        void PrettyPrint();

    private:
        void FindNeighborhoods();

        ComputationBufferSet neighborhoods_;
        ComputationBuffer last_separator_vertex_seen_;

        DISALLOW_DEFAULT_CONSTRUCTOR( SeparatorBlocks );
        DISALLOW_COPY_AND_ASSIGN( SeparatorBlocks );
};

}   // namespace chordalg

#endif // SEPARATOR_H
