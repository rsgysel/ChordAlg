#ifndef CONNECTEDCOMPONENTCALCULATOR_H
#define CONNECTEDCOMPONENTCALCULATOR_H

#include <algorithm>
#include <queue>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "utilities.h"

namespace chordalg {

typedef int ConnectedComponentID;

// Calculates the connected components of G - S for a graph G and vertex set S
// Intended for numerous computations on the same graph.
// Usage, given Graph G and VertexContainer S:
//
// ConnectedComponentCalculator CC(&G);
//
//
class ConnectedComponentCalculator
{
    public:
        explicit ConnectedComponentCalculator(Graph const&);
        ~ConnectedComponentCalculator() {};

        void Calculate(VertexContainer const &);

        inline unsigned int size() const {return size_;}
        inline ComputationBuffer const& S() const {return S_;}
        inline ConnectedComponentID kInSeparator() const {return -2;}
        inline ConnectedComponentID kUnsearched() const {return -1;}
        inline ConnectedComponentID operator[](Vertex v) const {return connected_component_[v];}

    private:
        void Init(VertexContainer const &);
        void FindComponents();

        Graph const& G_;
        ComputationBuffer S_;
        ComputationBuffer connected_component_;
        ComputationBuffer search_queue_;
        unsigned int size_;                                                     // # of connected components

        DISALLOW_DEFAULT_CONSTRUCTOR(ConnectedComponentCalculator);
        DISALLOW_COPY_AND_ASSIGN(ConnectedComponentCalculator);
};

class ConnectedComponentNeighborhoodCalculator
{
    public:
        ConnectedComponentNeighborhoodCalculator(Graph const&, ConnectedComponentCalculator const&);
        ~ConnectedComponentNeighborhoodCalculator() {};

        inline unsigned int size() const {return size_;}
        inline ComputationBuffer const& operator[](Vertex v) const {return neighborhood_[v];}

        void Calculate();

    private:
        Graph const& G_;
        ConnectedComponentCalculator const& CC_;
        ComputationBufferSet neighborhood_;
        unsigned int size_;

        void Init();
        void FindNeighborhoods();

        DISALLOW_DEFAULT_CONSTRUCTOR(ConnectedComponentNeighborhoodCalculator);
        DISALLOW_COPY_AND_ASSIGN(ConnectedComponentNeighborhoodCalculator);
};

}   // namespace chordalg

#endif // CONNECTEDCOMPONENTCALCULATOR_H
