#include "vertex_utilities.h"


namespace chordalg {

////////////// VertexPairs
////////////// ctor
//

VertexPairsIterator::VertexPairsIterator( const Vertices* const V, int begin, int end ) :
    V_  (   V       ),
    v1_ (   begin   ),
    v2_ (   begin   )
{
    if( V->size() == 1 )
        v1_ = v2_ = V->size();
    else if( begin != end )
        v2_++;

    return;
}

////////////// Iterator
//

void VertexPairsIterator::operator++()
{
    int n = V_->size();

    if( v1_ == n )
        return;

    if( v2_ == n - 1 )
    {
        ++v1_;

        if( v1_ == n - 1 )
            v1_ = v2_ = n;
        else
            v2_ = v1_ + 1;
    }
    else
        ++v2_;

    return;
}

} // namespace chordalg
