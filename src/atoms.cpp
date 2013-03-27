#include "atoms.h"

namespace chordalg {

Atoms::Atoms(Graph& G) : G_(G) {
    MCSmPlus();
    ComputeAtoms();
    return;
}

Atoms::~Atoms() {}

// Adaptation of MCS-M+, which minimally triangulates a graph.
// Described for atom computation by Berry, Pogorelcnik, and Simonet.
// paper: http://www.mdpi.com/1999-4893/3/2/197
//
//
// @result: clique minimal separators of G_
void Atoms::MCSmPlus(){

    int s = -1;				                // as in paper: for finding minimal separator generators
    int n = G_.order();

	std::vector<int> label;	                // as in paper
	label.resize(n);
	for( Vertex v : label )
		label[v] = 0;
    const int kDeleted = -1;                // label for deleted vertices

    std::vector< bool > reached;            // as in paper
    reached.resize(n);

    F_.resize(n);
    alpha_.resize(n);
    alpha_inverse_.resize(n);

	for( int i = n - 1; i >= 0; --i )
	{
	    std::vector<int>::iterator max_itr = std::max_element( label.begin(), label.end() );
	    int max_label = *max_itr;
	    Vertex x = std::distance( label.begin(), max_itr );

		if( max_label <= s )
			minsep_generators_.push_front(i);
		s = max_label;

		std::list<int> Y;	                // as in paper

		label[x] = kDeleted;
		for(  Vertex v : G_ )
            reached[v] = false;
		reached[x] = true;

        std::vector< std::list< Vertex > > reach;
        reach.resize(n);

		for( Vertex y : G_.N(x) )
		{
			if( label[ y ] != kDeleted )
			{
				reached[y] = true;
				reach[ label[y] ].push_back(y);
				Y.push_back(y);
			}
		}

		for( int j = 0; j < n - 1; ++j )
		{
			while( !reach[j].empty() )
			{
				Vertex y = reach[j].back(); reach[j].pop_back();
                for( Vertex z : G_.N(y) )
                {
                    if( label[z] != kDeleted && !reached[z] )
                    {
						reached[z] = true;
						reach[ label[z] ].push_back(z);

						if( label[z] > j )
						{
							Y.push_back(z);
							reach[ label[z] ].push_back(z);
						}
						else
							reach[j].push_back(z);
                    }
                }
			}
		}

		for( Vertex y : Y )
		{
			label[y]++;
			if( !G_.HasEdge(x,y) )
			{
			    F_[x].push_back(y);
			    F_[y].push_back(x);
			}
		}

		alpha_[i] = x;
		alpha_inverse_[x] = i;
	}

    return;
}

void Atoms::ComputeAtoms(){
    std::list< VertexContainer > vertices_of_atoms;
    SeparatorComponents cc(G_);

    VertexContainer deleted_vertices;                   // in paper, this is V(G_) - V(G_')
    std::vector< bool > is_deleted(G_.order(),false);

    int last_deleted;                                   // track index of last deleted vertex, for adding last atom
    // Examine minsep_generators, alpha_-earliest first
    for( int i : minsep_generators_ )
    {
        // Check to see if minimal separator is a cliques
        Vertex v = alpha_[i];

        std::vector< Vertex > S;
        for(Vertex u : G_.N(v))
        {
            if( alpha_inverse_[v] < alpha_inverse_[u] && !is_deleted[u] )
                S.push_back(u);
        }
        for(Vertex u : F_[v])
        {
            if( alpha_inverse_[v] < alpha_inverse_[u] && !is_deleted[u] )
                S.push_back(u);
        }

        if( G_.HasClique(S) )
        {
            last_deleted = i;
            for( Vertex u : deleted_vertices )
                S.push_back(u);

            cc.Set(S);
            VertexContainer C = cc.ConnectedComponent(v);
            VertexContainer atom;

            for( Vertex u : C )
            {
                if( !is_deleted[u] )
                {
                    deleted_vertices.push_back(u);
                    is_deleted[u] = true;
                    atom.push_back(u);
                }
            }

            for( Vertex u : S )
            {
                if( !is_deleted[u] )
                    atom.push_back(u);
            }

            vertices_of_atoms.push_back(atom);
        }
    }

    // Remaining vertices form an atom
    VertexContainer C;
    for( int i = last_deleted + 1; i < G_.order(); ++i )
        C.push_back(alpha_[i]);
    if(!C.empty())
        vertices_of_atoms.push_back(C);

    for( VertexContainer U : vertices_of_atoms )
        atom_subgraphs_.push_back( new InducedSubgraph(G_,U) );

    return;
}

} // namespace chordalg
