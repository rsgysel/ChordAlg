#ifndef GRAPH_H
#define GRAPH_H

#include "chordalg_types.h"
#include "file_reader.h"
#include "macros.h"

namespace chordalg {

class Graph;

class GraphIterator{
    public:
        GraphIterator(int graph_id) : graph_id_(graph_id), v_(0) {};
        GraphIterator(int graph_id, Vertex v) : graph_id_(graph_id), v_(v) {};

        // implemented for range-based for loops on the graph
        inline void operator++(){++v_;}
        inline bool operator==(const GraphIterator& other) const
            {return (graph_id_ == other.graph_id_) && (v_ == other.v_);}
        inline bool operator!=(const GraphIterator& other) const {return !(*this == other);}
        inline Vertex operator*() const {return v_;}
    private:
        const int graph_id_;
        Vertex v_;
};  // GraphVertexIterator

class Graph
{
    public:
        Graph(FileReader*);
        virtual ~Graph();

        // stat accessors
        inline int order() const {return order_;}
        inline int size() const {return size_;}

        // "iteration" accessors
        inline GraphIterator begin() const {return GraphIterator(graph_id_);}
        inline GraphIterator end() const {return GraphIterator(graph_id_,order_);}

        // Neighborhood accessor
        inline const VertexContainer& N(Vertex v) const {return neighborhoods_->operator[](v);}

        // Name accessor
        inline const VertexName& vertex_names(Vertex v) const {return vertex_names_->operator[](v);}

    protected:
        const AdjacencyLists* neighborhoods_;
        const VertexNameContainer* vertex_names_;

        unsigned int order_, size_;     // #vertices, #edges
        int graph_id_;                  // id = memory location

    private:
        DISALLOW_DEFAULT_CONSTRUCTOR(Graph);
        DISALLOW_COPY_AND_ASSIGN(Graph);
};  // Graph

} // namespace chordalg

#endif // GRAPH_H
