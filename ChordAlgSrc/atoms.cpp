#include "ChordAlgSrc/atoms.h"

#include <algorithm>
#include <list>
#include <vector>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs_m.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

//////////////////
// c'tors & d'tors

Atoms::Atoms(const Graph* G) :
    G_(G),
    clique_minimal_separators_(G_->order()) {
    return;
}

Atoms::~Atoms() {
    for (Graph* a : atom_subgraphs_) {
        delete a;
    }
    return;
}

////////
// Atoms

Atoms* Atoms::New(const Graph* G) {
    Atoms* A = new Atoms(G);
    A->ComputeAtoms();
    return A;
}

const LexTrie& Atoms::clique_minimal_separators() const {
    return clique_minimal_separators_;
}
int Atoms::size() {
    return atom_subgraphs_.size();
}

AtomIterator Atoms::begin() {
    return atom_subgraphs_.begin();
}
AtomIterator Atoms::end() {
    return atom_subgraphs_.end();
}

void Atoms::ComputeAtoms() {
    // Get minimal triangulation
    EliminationOrder* eo = new EliminationOrder(G_);
    FillEdges* F = new FillEdges(G_);
    VertexList* minsep_generators = new VertexList();
    MCSmPlus::Run(G_, eo, F, minsep_generators);

    std::list< Vertices > vertices_of_atoms;
    SeparatorComponents cc(G_);
    Vertices deleted_vertices;  // in paper, this is V(G_) - V(G_')
    std::vector< bool > is_deleted(G_->order(), false);

    // Examine minsep_generators, eo-earliest first
    for (int i : *minsep_generators) {
        // Check to see if minimal separator is a clique
        Vertex v = eo->VertexAt(i);
        Vertices S;
        for (Vertex u : G_->N(v)) {
            if (eo->Before(v, u) && !is_deleted[u]) {
                S.push_back(u);
            }
        }
        for (Vertex u : (*F)[v]) {
            if (eo->Before(v, u) && !is_deleted[u]) {
                S.push_back(u);
            }
        }
        if (G_->HasClique(S)) {
            clique_minimal_separators_.Insert(&S);
            for (Vertex u : deleted_vertices) {
                S.push_back(u);
            }
            cc.Separate(&S);
            Vertices C = cc.ConnectedComponent(v);
            Vertices atom;
            for (Vertex u : C) {
                if (!is_deleted[u]) {
                    deleted_vertices.push_back(u);
                    is_deleted[u] = true;
                    atom.push_back(u);
                }
            }
            for (Vertex u : S) {
                if (!is_deleted[u]) {
                    atom.push_back(u);
                }
            }
            vertices_of_atoms.push_back(atom);
        }
    }
    // Remaining vertices form an atom
    Vertices C;
    for (Vertex v : *G_) {
        if (!is_deleted[v]) {
            C.push_back(v);
        }
    }
    if (!C.empty()) {
        vertices_of_atoms.push_back(C);
    }
    for (Vertices U : vertices_of_atoms) {
        std::sort(U.begin(), U.end());
        atom_subgraphs_.push_back(new InducedSubgraph(G_, &U));
    }
    delete eo;
    delete F;
    delete minsep_generators;
    return;
}

}  // namespace chordalg

