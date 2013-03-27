#ifndef ATOMS_H_INCLUDED
#define ATOMS_H_INCLUDED

#include<algorithm> // delete me later
#include<iostream>
#include<iterator>
#include<list>
#include<map>
#include<vector>

#include"chordalg_types.h"
#include"graph.h"
#include"lex_trie.h"
#include"separator.h"

namespace chordalg {

void MCS_M_plus(Graph G);

class Atoms{
    public:
        Atoms(Graph& G);
        ~Atoms();

        int size(){return atom_subgraphs_.size();}

        AtomIterator begin(){ return atom_subgraphs_.begin(); }
        AtomIterator end(){ return atom_subgraphs_.end(); }

    private:
        void MCSmPlus();
        void ComputeAtoms();

        std::vector< AtomSubgraph* > atom_subgraphs_;

        Graph& G_;
        VertexContainer alpha_;                     // peo. alpha[i] is i^th vertex eliminated
        std::vector< int > alpha_inverse_;
        std::vector< std::list< Vertex > > F_;      // as in paper; minimal fill
        std::list< Vertex > minsep_generators_;     // 'X' in paper

        DISALLOW_DEFAULT_CONSTRUCTOR(Atoms);
        DISALLOW_COPY_AND_ASSIGN(Atoms);
};

} // namespace chordalg

#endif // ATOMS_H_INCLUDED
