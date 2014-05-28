#include "minimal_separator_algorithms.h"

namespace chordalg {

void PrettyPrintMinimalSeparators(const LexTrie& minseps, const Graph& G)
{
    for(Subset S : minseps)
        G.PrettyPrint(S);
    return;
}

// Berry, Bordat, and Cogis' algorithm to generate minimal separators
// paper: http://www.worldscientific.com/doi/pdf/10.1142/S0129054100000211
LexTrie* MinimalSeparators( Graph G )
{
    bool new_set;
    int n = G.order();
    LexTrie* minimal_separators = new LexTrie( n );

    // minimal separators not yet processed. entries are sorted
    std::vector< Vertices > minimal_separator_queue;

    // current vertex separator
    SeparatorBlocks S( G );

    // first phase: the neighborhood of each connected component
    // of G - N[v] for each vertex v is a minimal separator
    for( Vertex v : G )
    {
        S.SeparateClosedNbhd( v );
        for( Block B : S )
        {
            minimal_separators->SortedInsert< Vertices >( B.NC(), new_set );
            if( new_set )
                minimal_separator_queue.push_back( B.NC() );
        }
    }

    // second phase: for each v in S, the neighborhood of each
    // connected components of G - S U N[v] is a minimal separator
    Vertices V( n );
    while( !minimal_separator_queue.empty() )
    {
        Vertices U = minimal_separator_queue.back();
        minimal_separator_queue.pop_back();

        for( Vertex v : U )
        {
            V.merge(U, G.N( v ));
            S.Separate( V );

            for( Block B : S )
            {
                minimal_separators->SortedInsert< Vertices >( B.NC(), new_set );
                if( new_set )
                    minimal_separator_queue.push_back( B.NC() );
            }
        }
    }

    return minimal_separators;
}

LexTrie* MinimalSeparators( Graph G, Vertex a, Vertex b )
{
    bool new_set;
    int n = G.order();
    LexTrie* minimal_separators = new LexTrie( n );

    // minimal separators not yet processed. entries are sorted
    std::vector< Vertices > minimal_separator_queue;

    // current vertex separator
    Vertices        U( n );
    SeparatorBlocks S( G );

    // first phase: the neighborhood of the connected component
    // of G - N[a] containing b is a minimal ab-separator
    S.SeparateClosedNbhd(a);
    if( !S.IsInSeparator( b ) ){
        minimal_separators->SortedInsert< Vertices >( S.NComponentOf(b) );
        minimal_separator_queue.push_back( S.NComponentOf(b) );
    }

    // second phase: for each v in S, the neighborhood of each
    // connected components of G - S U N[v] is a minimal separator
    Vertices V( n );
    while( !minimal_separator_queue.empty() )
    {
        Vertices U = minimal_separator_queue.back();
        minimal_separator_queue.pop_back();
        for( Vertex v : U )
        {
            V.merge(U, G.N(v));
            S.Separate( V );
            if( !S.IsInSeparator(b) && !S.NComponentOf(b).empty() )
            {
                minimal_separators->SortedInsert< Vertices >( S.NComponentOf(b), new_set );
                if( new_set )
                    minimal_separator_queue.push_back( S.NComponentOf(b) );
            }
        }
    }
    return minimal_separators;
}

} // namespace chordalg
