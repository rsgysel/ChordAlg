#include "monochromatic_deficiency_heuristic.h"

namespace chordalg {

MonochromaticDeficiencyHeuristic::MonochromaticDeficiencyHeuristic(  ColoredIntersectionGraph& H ) :
    EliminationOrder( H ),
    H_ ( H )
{
    Init();
    return;
}

MonochromaticDeficiencyHeuristic::~MonochromaticDeficiencyHeuristic()
{
    return;
}

void MonochromaticDeficiencyHeuristic::Eliminate( Vertex v )
{
    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
    {
        if( !IsEdge( p ) )
        {
            fill_neighbors_[ p.first  ].insert( p.second );
            fill_neighbors_[ p.second ].insert( p.first  );
        }
    }

    return;
}

Weight MonochromaticDeficiencyHeuristic::WeightOf( Vertex v )
{
    Weight wt = 0;

    for( VertexPair p : VertexPairs( MonotoneNbhd( v ) ) )
    {
        if( !IsEdge( p ) )
        {
            Multicolor intersection;
            std::set_intersection(  H_.vertex_color( p.first  ).begin(),    H_.vertex_color( p.first  ).end(),
                                    H_.vertex_color( p.second ).begin(),    H_.vertex_color( p.second ).end(),
                                    std::inserter( intersection, intersection.begin() )
                                  );

            wt += intersection.size();
        }
    }

    return wt;
}

} // namespace chordalg
