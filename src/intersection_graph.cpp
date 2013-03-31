#include "intersection_graph.h"

namespace chordalg {

ColoredIntersectionGraph::ColoredIntersectionGraph( MatrixCellIntGraphFR* fr ) : Graph( fr ),
    subset_family_( fr->TakeSubsetFamily() ),
    subsets_( fr->subsets() ),
    vertex_colors_( fr->vertex_colors() )
{
    return;
}

ColoredIntersectionGraph::~ColoredIntersectionGraph()
{
    delete subset_family_;
    return;
}


void ColoredIntersectionGraph::PrettyPrintSubsets()
{
    for( Vertex v : *this )
    {
        std::cout << "C_" << v << ": ";
        std::copy( subsets_[v].begin(),subsets_[v].end(), std::ostream_iterator< Vertex >(std::cout, " ") );
        std::cout << std::endl;
        std::cout << "Colors: ";
        std::copy( vertex_colors_[v].begin(), vertex_colors_[v].end(), std::ostream_iterator< Color >(std::cout, " ") );
        std::cout << std::endl << std::endl;
    }
    return;
}

} // namespace chordalg
