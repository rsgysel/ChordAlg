#include "tree_representation.h"

namespace chordalg {

TreeRepresentation::TreeRepresentation( AdjacencyLists* E,  VertexNames K, Graph& G   ) :
    G_( G       ),
    T_( E, K    )
{
    //ctor
}

TreeRepresentation::~TreeRepresentation()
{
    //dtor
}

CliqueTree::CliqueTree( AdjacencyLists* E,  VertexNames K, Graph& G   ) : TreeRepresentation(E,K,G)
{
    //ctor
}

CliqueTree::~CliqueTree()
{
    //dtor
}

}; // namespace chordalg
