#ifndef GRAPH_H
#define GRAPH_H

#include "chordalg_types.h"
#include "file_reader.h"

namespace chordalg {

class Graph;

class GraphIterator{
    public:
        GraphIterator(int GraphMemLoc) : GraphMemLoc_(GraphMemLoc), v_(0) {};
        GraphIterator(int GraphMemLoc, Vertex v) : GraphMemLoc_(GraphMemLoc), v_(v) {};

        void operator++(){++v_;}
        bool operator==(const GraphIterator& other) const {return (GraphMemLoc_ == other.GraphMemLoc_) && (v_ == other.v_);}
        bool operator!=(const GraphIterator& other) const {return !(*this == other);}
        Vertex operator*() const {return v_;}
    private:
        int GraphMemLoc_;
        Vertex v_;
};  // GraphVertexIterator

class Graph
{
    public:
        Graph(FileReader&);
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

        int order_, size_;  // #vertices, #edges
        int graph_id_;      // id = memory location

    private:
        Graph() : neighborhoods_(NULL), vertex_names_(NULL), size_(0), graph_id_(0) {};   // default constructor disabled
};  // Graph

} // namespace chordalg

#endif // GRAPH_H
