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

std::string TreeRepresentation::SerializeMaxcliqueAsString( Vertices K ) const
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

void TreeRepresentation::PhyloNewickPrint( const ColoredIntersectionGraph& cig ) const
{
    // DFS info
    std::string newick_tree;
    VertexSet   visited;
    Vertex      v = *(T_.begin());
    visited.insert(v);

    // Taxon Clique info
    std::vector< int > taxon_clique_size(cig.subset_family()->n(), 0);
    for(Vertex v : cig)
    {
        Subset S = cig.subset(v);
        for(Element e : S)
            ++taxon_clique_size[e];
    }

    // Start DFS
    PhyloNewickVisit(visited, v, newick_tree, cig, taxon_clique_size);
    if(visited.size() != T_.order())
    {
        std::cerr << "Error in TreeRepresentation::PhyloNewickPrint: not all nodes visited" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << newick_tree << ";" << std::endl;
    return;
}

void TreeRepresentation::PhyloNewickVisit(  VertexSet&                      visited,
                                            Vertex                          v,
                                            std::string&                    newick_tree,
                                            const ColoredIntersectionGraph& cig,
                                            std::vector< int >&             taxon_clique_size ) const
{
    visited.insert(v);
    bool is_leaf = true;
    newick_tree += std::string("(");
    std::string subtree;
    // Recurse
    for(Vertex u : T_.N(v))
    {
        if(visited.find(u) == visited.end())
        {
                is_leaf = false;
                PhyloNewickVisit(visited, u, subtree, cig, taxon_clique_size);
                subtree += std::string(",");
        }
    }
    // Calculate candidate-ness of node
    std::vector< int > cell_count( taxon_clique_size.size(), 0 );
    for(Vertex v : clique_map_[v])
    {
        for(Element e : cig.subset(v))
            ++cell_count[e];
    }
    std::string node_name;
    bool taxon_label = false;
    for(int i = 0; i < cell_count.size(); ++i)
    {
        if(cell_count[i] == taxon_clique_size[i])
        {
            node_name += cig.taxon_name(i) + std::string(" ");
            taxon_label = true;
        }
    }
    if(taxon_label)
        node_name.erase(node_name.end()-1);

// TODO: Print as Multree, only leaves are labeled. So if node_name is non-empty and internal, add a leaf.
    // Leaf or Internal node
    if(is_leaf)
    {
        newick_tree.erase(newick_tree.end()-1);   // no () for leaf node
        newick_tree += subtree + node_name;
    }
    else
    {
        subtree.erase(subtree.end()-1);    // trailing comma
        newick_tree += subtree + std::string(")") + node_name;
    }
    return;
}

}; // namespace chordalg
