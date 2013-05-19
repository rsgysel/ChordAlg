#include "minimal_separator_algorithms.h"

namespace chordalg {

// Berry, Bordat, and Cogis' algorithm to generate minimal separators
// paper: http://www.worldscientific.com/doi/pdf/10.1142/S0129054100000211

LexTrie& BerryBordatCogis( Graph G )
{
    bool new_set;
    int n = G.order();
    LexTrie* minimal_separators = new LexTrie( n );

    // minimal separators not yet processed. entries are sorted
    std::vector< Vertices > minimal_separator_queue;

    // current vertex separator
    Vertices        U( n );
    SeparatorBlocks S( G );

    // first phase: the neighborhood of each connected component
    // of G - N[v] for each vertex v is a minimal separator
    for( Vertex v : G )
    {
        U.clear();
        U.add( v );
        for( Vertex u : G.N( v ) )
            U.add( u );

        S.Separate( U );
        for( Vertices NC : S )
        {
            minimal_separators->Insert< Vertices >( NC, new_set );
            if( new_set )
                minimal_separator_queue.push_back( NC );
        }
    }

    // second phase: for each v in S, the neighborhood of each
    // connected components of G - S U N[v] is a minimal separator
    Vertices V( n );
    while( !minimal_separator_queue.empty() )
    {
        U = minimal_separator_queue.back();
        minimal_separator_queue.pop_back();

        for( Vertex v : U )
        {
            V.clear();
            std::merge( U.begin(), U.end(),
                        G.N( v ).begin(), G.N( v ).end(),
                        V.begin());

            S.Separate( V );

            for( Vertices NC : S )
            {
                std::sort( NC.begin(), NC.end() );
                minimal_separators->Insert< Vertices >( NC, new_set );

                if( new_set )
                    minimal_separator_queue.push_back( NC );
            }
        }
    }

    return *minimal_separators;
}

}
