#include "elimination_order.h"

namespace chordalg {

// EliminationOrder
EliminationOrder::EliminationOrder( Graph& G ) :
    G_                  ( G             ),
    alpha_              ( G.order()     ),
    alpha_inverse_      ( G.order()     ),
    fill_count_         ( kUnfilled()   )
{
    Init();
    return;
}

EliminationOrder::~EliminationOrder()
{
    return;
}

void EliminationOrder::Init()
{
    int i = 0;
    for(Vertex v : G_)
    {
        alpha_[i] = v;
        alpha_inverse_[v] = i;
        ++i;
    }
    return;
}

void EliminationOrder::Emplace(Vertex v, int i)
{
    Swap(i, alpha_inverse_[v]);
    return;
}

void EliminationOrder::Swap(int i, int j)
{
    Vertex  vi = alpha_[i],
            vj = alpha_[j];
    std::swap(alpha_[i], alpha_[j]);
    std::swap(alpha_inverse_[vi], alpha_inverse_[vj]);
    return;
}

// Produces (and passes ownership of) adjacency lists
// form triagnulation neighborhoods
AdjacencyLists* EliminationOrder::TriangNbhds() const
{
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(G_.order());
    if(fill_count_ == 0)
    {
        for(Vertex v : G_)
        {
            a_lists->operator[](v).reserve(triangulation_nbhds_[v].size());
            for(Vertex u : G_.N(v))
                a_lists->operator[](v).add(u);
        }
    }
    else
    {
        for(Vertex v : G_)
        {
            a_lists->operator[](v).reserve(triangulation_nbhds_[v].size());
            for(Vertex u : triangulation_nbhds_[v])
                a_lists->operator[](v).add(u);
        }
    }
    return a_lists;
}


AdjacencyLists* EliminationAlgorithm::TriangNbhds() const
{
    AdjacencyLists* a_lists = new AdjacencyLists();
    a_lists->resize(G_.order());
    if(fill_count_ == 0)
    {
        for(Vertex v : G_)
        {
            a_lists->operator[](v).reserve(G_.N(v).size());
            for(Vertex u : G_.N(v))
                a_lists->operator[](v).add(u);
        }
    }
    else
    {
        for(Vertex v : G_)
        {
            a_lists->operator[](v).reserve(G_.N(v).size() + fill_neighbors_[v].size());
            for(Vertex u : G_.N(v))
                a_lists->operator[](v).add(u);
            for(Vertex u : fill_neighbors_[v])
                a_lists->operator[](v).add(u);
        }
    }
    return a_lists;
}

// Tarjan and Yannakakis' algorithm to compute fill edges from an elimination order, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test chordality of
// graphs, test acyclicity of hypergraphs, and selectively reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
int EliminationOrder::ComputeFill()
{
    if(fill_count_ != kUnfilled())
    {
        return fill_count_;
    }
    fill_count_ = 0;

    triangulation_nbhds_.resize( G_.order() );
    VertexVector follower;
    follower.resize(G_.order());
    VertexVector index;
    index.resize(G_.order());
    for(int i = 0; i < G_.order(); ++i)
    {
        Vertex w = VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for(Vertex v : LNbhd(w))
        {
            Vertex x = v;
            while(index[x] < i)
            {
                index[x] = i;
                ++fill_count_;
                triangulation_nbhds_[x].insert(w);
                triangulation_nbhds_[w].insert(x);
                x = follower[x];
            }
            if(follower[x] == x)
            {
                follower[x] = w;
            }
        }
    }
    fill_count_ -= G_.size();
    return fill_count_;
}

// Tarjan and Yannakakis' algorithm to check if an elimination order is perfect, from:
// R.E. Tarjan and M. Yannakakis. "Simple linear-time algorithms to test chordality of
// graphs, test acyclicity of hypergraphs, and selectively reduce acyclic hypergraphs".
// SIAM J. Comput., 13:566-579, 1984.
bool EliminationOrder::ZeroFill() const
{
    if(fill_count_ != kUnfilled())
    {
        return fill_count_ == 0;
    }
    VertexVector follower;
    follower.resize(G_.order());
    VertexVector index;
    index.resize(G_.order());
    for(int i = 0; i < G_.order(); ++i)
    {
        Vertex w = VertexAt(i);
        follower[w] = w;
        index[w] = i;
        for(Vertex v : LNbhd(w))
        {
            index[v] = i;
            if(follower[v] == v)
            {
                follower[v] = w;
            }
        }
        for(Vertex v : LNbhd(w))
        {
            if(index[follower[v]] < i)
            {
                return false;
            }
        }
    }
    return true;
}

Vertices EliminationOrder::LNbhd(Vertex v) const
{
    Vertices L_N;

    for( Vertex u : G_.N( v ) )
    {
        if(Before(u,v))
            L_N.add( u );
    }

    if(!triangulation_nbhds_.empty())
    {
        for( Vertex u : triangulation_nbhds_[ v ] )
        {
            if(Before(u,v))
                L_N.add( u );
        }
    }

    return L_N;
}

Vertices EliminationOrder::RNbhd(Vertex v) const
{
    Vertices R_N;

    for( Vertex u : G_.N( v ) )
    {
        if(Before(v,u))
            R_N.add( u );
    }

    if(!triangulation_nbhds_.empty())
    {
        for( Vertex u : triangulation_nbhds_[ v ] )
        {
            if(Before(v,u))
                R_N.add( u );
        }
    }

    return R_N;
}

void EliminationOrder::SetOrder(VertexVector pi)
{
    if(pi.size() != G_.order())
    {
        std::cerr << "Error in SetOrder: elimination order size and graph order does not match" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < pi.size(); ++i)
        Emplace(pi[i], i);
    return;
}

void EliminationOrder::PrettyPrint() const
{
    std::cout << "elimination order:\t" ;
    for( Vertex v : alpha_ )
        std::cout << G_.name( v ) << " ";
    std::cout << std::endl;
    return;
}

// EliminationAlgorithm
EliminationAlgorithm::EliminationAlgorithm( Graph& G ) :
    G_                  ( G         ),
    alpha_              ( G.order() ),
    alpha_inverse_      (           ),
    fill_cost_          ( 0         ),
    fill_count_         ( 0         ),
    fill_neighbors_     (           ),
    remaining_vertices_ (           ),
    ties_               (           ),
    tie_count_          (           )
{
    srand( time(NULL) );
    return;
}

EliminationAlgorithm::~EliminationAlgorithm()
{
    return;
}

void EliminationAlgorithm::Init()
{
    int n = G_.order();

    alpha_inverse_.resize   ( n );
    fill_neighbors_.resize  ( n );
    tie_count_.resize       ( n );

    for( Vertex v : G_ )
        remaining_vertices_.insert( v );

    Elimination();

    return;
}

void EliminationAlgorithm::Elimination()
{
    for( int i = 0; i < G_.order(); ++i )
    {
        VertexCost vc   =   ArgMin();
        fill_cost_      +=  vc.second;
        Vertex v        =   vc.first;

        alpha_          [ i ] = v;
        alpha_inverse_  [ v ] = i;

        tie_count_      [ i ] = ties_.size();
        ties_.clear();

        remaining_vertices_.erase   ( v );
        Eliminate                   ( v );
    }

    return;
}

void EliminationAlgorithm::AddEdge( VertexPair p )
{
    if( !IsEdge( p ) )
    {
        fill_neighbors_[ p.first  ].insert( p.second );
        fill_neighbors_[ p.second ].insert( p.first  );
        ++fill_count_;
    }

    return;
}

bool EliminationAlgorithm::IsEdge( VertexPair p )
{
    return G_.HasEdge ( p ) || IsFillEdge( p );
}

bool EliminationAlgorithm::IsFillEdge( VertexPair p )
{
    return fill_neighbors_[ p.first ].find( p.second ) != fill_neighbors_[ p.first ].end();
}

bool EliminationAlgorithm::IsRemoved( Vertex v )
{
    return remaining_vertices_.find( v ) == remaining_vertices_.end();
}

void EliminationAlgorithm::Saturate( Vertices U )
{
    for( VertexPair p : VertexPairs( U ) )
    {
        if( !IsEdge( p ) )
        {
            Vertex  u = p.first,
                    v = p.second;
            fill_neighbors_[ u ].insert( v );
            fill_neighbors_[ v ].insert( u );
        }
    }

    return;
}

VertexCost EliminationAlgorithm::ArgMin()
{
    Weight min = MAX_WEIGHT;

    for( Vertex v : remaining_vertices_ )
    {
        std::pair< Weight, Cost >   wc = WeightOf( v );
        Weight                      wt = wc.first;
        Cost                        c  = wc.second;

        if( wt < min )
        {
            min = wt;
            ties_.clear();
            ties_.push_back( VertexCost( v, c ) );
        }
        else if( wt == min )
        {
            ties_.push_back( VertexCost( v, c ) );
        }
    }

    return TieBreak();
}

VertexCost EliminationAlgorithm::TieBreak()
{
    int i = rand() % ties_.size();
    return ties_[ i ];
}

Vertices EliminationAlgorithm::MonotoneNbhd( Vertex v )
{
    Vertices N_alpha;

    for( Vertex u : G_.N( v ) )
    {
        if( !IsRemoved( u ) )
            N_alpha.add( u );
    }

    for( Vertex u : fill_neighbors_[ v ] )
    {
        if( !IsRemoved( u ) )
            N_alpha.add( u );
    }

    return N_alpha;
}

void EliminationAlgorithm::PrettyPrint() const
{
    std::cout << "elimination order:\t" ;
    for( Vertex v : alpha_ )
        std::cout << G_.name( v ) << " ";
    std::cout << std::endl;

    std::cout << "tie distribution:\t"  ;
    std::copy( tie_count_.begin(), tie_count_.end(), std::ostream_iterator< int >( std::cout, " " ) );
    std::cout << std::endl;

    return;
}

} // namespace chordalg
