#include "tree_representation.h"

namespace chordalg {

TreeRepresentation::TreeRepresentation( AdjacencyLists* E, Graph& G, std::vector< Vertices > clique_map ) :
    G_          ( G                                 ),
    T_          ( E, NamesFromCliqueMap(clique_map) ),
    clique_map_ ( clique_map                        )
{
    //ctor
}

TreeRepresentation::~TreeRepresentation()
{
    //dtor
}

std::string TreeRepresentation::SerializeMaxcliqueAsString(Vertices K) const
{
    std::string maxclique("{");
    for(Vertex v : K)
        maxclique += G_.name(v) + std::string(" ");
    maxclique.erase(maxclique.end()-1, maxclique.end());
    maxclique += std::string("}");
    return maxclique;
}

VertexNames TreeRepresentation::NamesFromCliqueMap( std::vector< Vertices > clique_map ) const
{
    VertexNames names;
    names.resize(clique_map.size());
    for( Vertex v = 0; v < clique_map.size(); ++v )
        names[v] = SerializeMaxcliqueAsString( clique_map[v] );
    return names;
}

CliqueTree::CliqueTree( AdjacencyLists* E, Graph& G, std::vector< Vertices > clique_map ) : TreeRepresentation(E,G,clique_map)
{
    //ctor
}

CliqueTree::~CliqueTree()
{
    //dtor
}

// Prints tree as a Newick notation / New Hampshire tree format.
// ``string" may not contain semicolon, parentheses, comma, or colon
//
// Newick files are given by the grammar:
//   Tree --> Subtree ";"
//   Subtree --> Leaf | Internal
//   Leaf --> Name
//   Internal --> "(" BranchList ")" Name
//   BranchList --> Branch | Branch "," BranchList
//   Branch --> Subtree Length
//   Name --> empty | string
//   Length --> empty | ":" number
//
void TreeRepresentation::NewickPrint() const
{
    std::string newick_tree;
    VertexSet   visited;
    Vertex      v = *(T_.begin());
    visited.insert(v);
    NewickVisit(visited, v, newick_tree);
    if(visited.size() != T_.order())
    {
        std::cerr << "Error in TreeRepresentation::NewickPrint: not all nodes visited" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << newick_tree << ";" << std::endl;
    return;
}

void TreeRepresentation::NewickVisit(VertexSet& visited, Vertex v, std::string& newick_tree) const
{
    visited.insert(v);
    bool is_leaf = true;
    newick_tree += std::string("(");
    std::string subtree;
    for(Vertex u : T_.N(v))
    {
        if(visited.find(u) == visited.end())
        {
                is_leaf = false;
                NewickVisit(visited, u, subtree);
                subtree += std::string(",");
        }
    }
    if(is_leaf)
    {
        newick_tree.erase(newick_tree.end()-1);   // no () for leaf node
        newick_tree += subtree + T_.name(v);
    }
    else
    {
        subtree.erase(subtree.end()-1);    // trailing comma
        newick_tree += subtree + std::string(")") + T_.name(v);
    }
    return;
}

}; // namespace chordalg
