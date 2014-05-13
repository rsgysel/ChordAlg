/*
 *  atoms.hpp - computes atom subgraphs
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  Given a graph G, atoms returns the subgraphs of G induced by its atoms.
 *  This is known as the clique-separator decomposition of a graph. See
 *  Berry, Pogorelcnik, and Simonet "An introduction to clique minimal
 *  separator decomposition"
 */

#ifndef ATOM_SUBGRAPHS_HPP
#define ATOM_SUBGRAPHS_HPP

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <vector>

#include "chordalg_types.h"
#include "graph.h"
#include "lex_trie.h"
#include "separator.h"
#include "vertex_utilities.h"

namespace chordalg {

template< class GraphType >
class Atoms
{
    public:
        Atoms( GraphType& G );
        ~Atoms();

        int size(){ return atom_subgraphs_.size(); }

        typedef typename std::vector< GraphType* >::const_iterator AtomIterator;
        AtomIterator begin()  { return atom_subgraphs_.begin(); }
        AtomIterator end()    { return atom_subgraphs_.end(); }

    private:
        void MCSmPlus();
        void ComputeAtoms();

        std::vector< GraphType* > atom_subgraphs_;

        GraphType&                G_;
        VertexVector              alpha_; // peo. alpha[i] is i^th vertex eliminated
        std::vector< int >        alpha_inverse_;
        std::vector< VertexList > F_; // as in paper; minimal fill
        VertexList                minsep_generators_; // 'X' in paper

        // Disable default constructor, copy constructor, assignment
        Atoms();
        Atoms(const Atoms&);
  void operator=(const Atoms&);
};

template< class GraphType >
Atoms< GraphType >::Atoms( GraphType& G ) : G_( G )
{
    MCSmPlus();
    ComputeAtoms();
    return;
}

template< class GraphType >
Atoms< GraphType >::~Atoms()
{
    for( GraphType* a : atom_subgraphs_ )
        delete a;
    return;
}

// Implementation of MCS-M+, which minimally triangulates a graph.
// Described for atom computation by Berry, Pogorelcnik, and Simonet.
// paper: http://www.mdpi.com/1999-4893/3/2/197
//
//
// @result: clique minimal separators of G_
template< class GraphType >
void Atoms< GraphType >::MCSmPlus()
{
    int s = -1;				                // as in paper: for finding minimal separator generators
    int n = G_.order();

	std::vector< int > label;	            // as in paper
	label.resize( n );
	for( Vertex v : label )
		label[ v ] = 0;
    const int kDeleted = -1;                // label for deleted vertices

    std::vector< bool > reached;            // as in paper
    reached.resize( n );

    F_.resize( n );
    alpha_.resize( n );
    alpha_inverse_.resize( n );

	for( int i = n - 1; i >= 0; --i )
	{
	    std::vector< int >::iterator max_itr = std::max_element( label.begin(), label.end() );
	    int max_label = *max_itr;
	    Vertex x = std::distance( label.begin(), max_itr );

		if( max_label <= s )
			minsep_generators_.push_front( i );
		s = max_label;

		std::list< int > Y;	                // as in paper

		label[ x ] = kDeleted;
		for(  Vertex v : G_ )
            reached[ v ] = false;
		reached[ x ] = true;

        std::vector< std::list< Vertex > > reach;
        reach.resize( n );

		for( Vertex y : G_.N( x ) )
		{
			if( label[ y ] != kDeleted )
			{
				reached[ y ] = true;
				reach[ label[ y ] ].push_back( y );
				Y.push_back( y );
			}
		}

		for( int j = 0; j < n - 1; ++j )
		{
			while( !reach[ j ].empty() )
			{
				Vertex y = reach[ j ].back(); reach[ j ].pop_back();
                for( Vertex z : G_.N( y ) )
                {
                    if( label[ z ] != kDeleted && !reached[ z ] )
                    {
						reached[ z ] = true;
						reach[ label[ z ] ].push_back( z );

						if( label[ z ] > j )
						{
							Y.push_back( z );
							reach[ label[ z ] ].push_back( z );
						}
						else
							reach[ j ].push_back( z );
                    }
                }
			}
		}

		for( Vertex y : Y )
		{
			label[ y ]++;
			if( !G_.HasEdge( x, y ) )
			{
			    F_[ x ].push_back( y );
			    F_[ y ].push_back( x );
			}
		}

		alpha_[ i ] = x;
		alpha_inverse_[ x ] = i;
	}

    return;
}

template< class GraphType >
void Atoms< GraphType >::ComputeAtoms()
{
    std::list< Vertices >   vertices_of_atoms;
    SeparatorComponents     cc                  ( G_ );
    Vertices                deleted_vertices;                               // in paper, this is V(G_) - V(G_')
    std::vector< bool >     is_deleted          ( G_.order(), false );
    int                     last_deleted;                                   // track index of last deleted vertex, for adding last atom

    // Examine minsep_generators, alpha_-earliest first
    for( int i : minsep_generators_ )
    {
        // Check to see if minimal separator is a cliques
        Vertex v = alpha_[ i ];

        Vertices S;
        for( Vertex u : G_.N( v ) )
        {
            if( alpha_inverse_[ v ] < alpha_inverse_[ u ] && !is_deleted[ u ] )
                S.add( u );
        }
        for( Vertex u : F_[ v ] )
        {
            if( alpha_inverse_[ v ] < alpha_inverse_[ u ] && !is_deleted[ u ] )
                S.add( u );
        }

        if( G_.HasClique( S ) )
        {
            last_deleted = i;
            for( Vertex u : deleted_vertices )
                S.add( u );

            cc.Separate( S );
            Vertices C = cc.ConnectedComponent( v );
            Vertices atom;

            for( Vertex u : C )
            {
                if( !is_deleted[ u ] )
                {
                    deleted_vertices.add( u );
                    is_deleted[ u ] = true;
                    atom.add( u );
                }
            }

            for( Vertex u : S )
            {
                if( !is_deleted[ u ] )
                    atom.add( u );
            }

            vertices_of_atoms.push_back( atom );
        }
    }

    // Remaining vertices form an atom
    Vertices C;
    for( Vertex v : G_ )
        if( !is_deleted[ v ] )
            C.add( v );

    if( !C.empty() )
        vertices_of_atoms.push_back( C );

    for( Vertices U : vertices_of_atoms )
        atom_subgraphs_.push_back( new GraphType( G_, U ) );

    return;
}

} // namespace chordalg

#endif // ATOM_SUBGRAPHS_HPP
