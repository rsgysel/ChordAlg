#ifndef SEPARATOR_H_INCLUDED
#define SEPARATOR_H_INCLUDED

#include<algorithm>
#include<queue>
#include<vector>

#include"chordalg_types.h"
#include"graph.h"

namespace chordalg{

class Separator
{
    public:
        Separator(Graph* const);
        ~Separator();

        void Calculate(VertexContainer);

    private:
        const Graph* const G_;                                                  // Separator will never own G, do not delete
        std::vector< ConnectedComponentOf > component_of_vertex_;
        int size;                                                               // # of connected components

        static const ConnectedComponentOf in_separator_ = -2;
        static const ConnectedComponentOf unsearched_ = -1;
        static const int first_component_ = 0;

        Separator() : G_(NULL), component_of_vertex_(), size(0) {};
};

}   // namespace chordalg

#endif // SEPARATOR_H_INCLUDED
