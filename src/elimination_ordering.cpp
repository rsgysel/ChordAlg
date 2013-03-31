#include "elimination_ordering.h"

namespace chordalg {

EliminationOrdering::EliminationOrdering(Graph& G) : G_(G),
    fill_size_(0),
    fill_edge_count_(G_.order(), std::vector<int>(G_.order(), 0)),
    fill_neighbors_(G_.order(), std::map< Vertex, bool >())
{
    Init();
    return;
}

EliminationOrdering::EliminationOrdering(EliminationOrdering& eo) : G_(eo.G_),
    alpha_(eo.alpha_),
    alpha_inverse_(eo.alpha_inverse_)
{
    int n = G_.order();
    Refill(0,n - 1);
    return;
}

EliminationOrdering::~EliminationOrdering()
{
    //dtor
}

void EliminationOrdering::Init()
{
    int n = G_.order();
    alpha_.resize(n);
    alpha_inverse_.resize(n);

    int i = 0;
    for( Vertex v : G_ )
    {
        alpha_[i] = v;
        alpha_inverse_[v] = i;
        ++i;
    }

    Refill(0,n-1);
    return;
}

VertexContainer EliminationOrdering::MonotoneAdjacencySet(Vertex v)
{
    VertexContainer N_alpha;
    for( Vertex u : G_.N(v) )
    {
        if( alpha_inverse_[v] < alpha_inverse_[u] )
            N_alpha.push_back(u);
    }
    for( std::pair< Vertex, bool > p : fill_neighbors_[v] )
    {
        Vertex u = p.first;
        if( alpha_inverse_[v] < alpha_inverse_[u] )
            N_alpha.push_back(u);
    }
    return N_alpha;
}

void EliminationOrdering::PrettyPrint()
{
    std::cout << "eo: ";
    for( Vertex v : alpha_ )
        std::cout << G_.name(v) << " ";
    std::cout << std::endl;

    for( Vertex v : G_ )
        std::cout << "(" << alpha_inverse_[v] << ',' << G_.name(v) << ") ";
    std::cout << std::endl;

    std::cout << "fill edges: ";
    for( Vertex v : G_ )
    {
        for( std::pair< Vertex, bool > p : fill_neighbors_[v] )
        {
            Vertex u = p.first;
            if( v < u )
                std::cout << fill_edge_count_[u][v] << '.' << fill_edge_count_[v][u] << "x(" << G_.name(v) << ", " << G_.name(u) << ") ";
        }
    }
    std::cout << std::endl << std::endl;
}

void EliminationOrdering::Refill(int lower, int upper)
{
    for(int i = lower; i <= upper; ++i)
    {
        Vertex v = alpha_[i];
        VertexContainer N_alpha = MonotoneAdjacencySet(v);

        for( VertexIterator u_itr = N_alpha.begin(); u_itr != N_alpha.end(); ++u_itr )
        {
            VertexIterator w_itr = u_itr;
            for( ++w_itr; w_itr != N_alpha.end() ; ++w_itr )
            {
                Vertex u = *u_itr,  w = *w_itr;
                if( !G_.HasEdge(u,w) )
                {
                    if( fill_edge_count_[u][w] == 0 )
                        AddEdge(u,w);
                    fill_edge_count_[u][w]++;
                    fill_edge_count_[w][u]++;
                }
            }
        }
    }

    return;
}

void EliminationOrdering::MoveAndPush(int i, int j)
{
    Unfill(i,j);

    int delta = (i < j) ? 1 : -1;
    for(int k = i; k != j; k = k + delta)
        SwapPair(k,k + delta);

    Refill(i,j);
}

// Re-calculates triangulation between i and j (inclusive)
void EliminationOrdering::Swap(int i, int j)
{
    if(j < i)
        std::swap(i,j);

    Unfill(i,j);
    SwapPair(i,j);
    Refill(i,j);

    return;
}

void EliminationOrdering::SwapPair(int i, int j)
{
    Vertex vi = alpha_[i], vj = alpha_[j];
    std::swap(alpha_inverse_[vi], alpha_inverse_[vj]);
    std::swap(alpha_[i], alpha_[j]);
    return;
}

void EliminationOrdering::Unfill(int lower, int upper)
{

    for(int i = upper; i >= lower; --i)
    {
        Vertex v = alpha_[i];
        VertexContainer N_alpha = MonotoneAdjacencySet(v);
        for( VertexIterator u_itr = N_alpha.begin(); u_itr != N_alpha.end(); ++u_itr )
        {
            VertexIterator w_itr = u_itr;
            for( ++w_itr; w_itr != N_alpha.end() ; ++w_itr )
            {
                Vertex u = *u_itr,  w = *w_itr;
                if( !G_.HasEdge(u,w) )
                {
                    fill_edge_count_[u][w]--;
                    fill_edge_count_[w][u]--;
                    if( fill_edge_count_[u][w] == 0 )
                        RemoveEdge(u,w);
                }
            }
        }
    }
    return;
}

void EliminationOrdering::AddEdge(Vertex u, Vertex w){
    fill_size_++;
    fill_neighbors_[u][w] = fill_neighbors_[w][u] = true;
    return;
}

void EliminationOrdering::RemoveEdge(Vertex u, Vertex w){
    fill_size_--;
    fill_neighbors_[u].erase(w);
    fill_neighbors_[w].erase(u);
    return;
}

bool EliminationOrdering::IsOrderConsistent()
{
    for( Vertex v : G_ )
    {
        int i = alpha_inverse_[v];
        if( v != alpha_[i] )
            return false;
    }
    return true;
}

} // namespace chordalg
