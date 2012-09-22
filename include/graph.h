#ifndef GRAPH_H
#define GRAPH_H

#include "chordalg_types.h"
#include "file_reader.h"

namespace chordalg {

class Graph
{
    public:
        Graph(FileReader&);
        virtual ~Graph();

        // stat accessors
        inline int order() const {return order_;}
        inline int size() const {return size_;}

        // "iteration" accessors
        inline Vertex begin() const {return 0;}
        inline Vertex end() const {return order_ - 1;}

        // Neighborhood accessor
        //inline const VertexContainer* N_range_check(Vertex) const {return neighborhoods_->at(v);}
        inline const VertexContainer& N(Vertex v) const {return neighborhoods_->operator[](v);}

        // Name accessor
        inline const VertexName& vertex_names(Vertex v) const {return vertex_names_->operator[](v);}

    protected:
        const AdjacencyLists* neighborhoods_;
        const VertexNameContainer* vertex_names_;

        int order_, size_;  // #vertices, #edges

    private:
        Graph() : neighborhoods_(NULL), vertex_names_(NULL), size_(0) {};   // default constructor disabled
};  // Graph

} // namespace chordalg

#endif // GRAPH_H
