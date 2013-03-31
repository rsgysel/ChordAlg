#ifndef ELIMINATION_ORDERING_H
#define ELIMINATION_ORDERING_H

#include<algorithm>
#include<map>
#include<vector>
#include<iostream>
#include<iterator>

#include "graph.h"

namespace chordalg{

class EliminationOrdering
{
    public:
        EliminationOrdering(Graph& G);
        EliminationOrdering(EliminationOrdering& eo);
        virtual ~EliminationOrdering();

        int fill_size(){ return fill_size_; }
        VertexContainer Ordering(){ return alpha_; }

        bool IsOrderConsistent();
        void PrettyPrint();

        void MoveAndPush(int, int);
        void Swap(int, int);

    protected:
        void Init();

        VertexContainer MonotoneAdjacencySet(Vertex);
        void Refill(int, int);
        void Unfill(int, int);
        void AddEdge(Vertex, Vertex);
        void RemoveEdge(Vertex, Vertex);
        void SwapPair(int,int);

        Graph& G_;
        VertexContainer alpha_;             // alpha[i] = ith vertex eliminated
        std::vector< int > alpha_inverse_;  // alpha_inverse[v] = elimination # of v

        int fill_size_;
        std::vector< std::vector< int > > fill_edge_count_; // # of vertices
        std::vector< std::map< Vertex, bool > > fill_neighbors_;
};


} // namespace chordalg

#endif // ELIMINATION_ORDERING_H
