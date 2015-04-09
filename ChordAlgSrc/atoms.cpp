#include "atoms.h"

namespace chordalg {

Atoms::Atoms(const Graph* G) : G_(G) {
    MCSmPlus(*G_, alpha_, alpha_inverse_, F_, minsep_generators_);
    ComputeAtoms();
    return;
}

Atoms::~Atoms() {
    for (Graph* a : atom_subgraphs_) {
        delete a;
    }
    return;
}

void Atoms::ComputeAtoms() {
    std::list< Vertices > vertices_of_atoms;
    SeparatorComponents cc(*G_);
    Vertices deleted_vertices;  // in paper, this is V(G_) - V(G_')
    std::vector< bool > is_deleted(G_->order(), false);

    // Examine minsep_generators, alpha_-earliest first
    for (int i : minsep_generators_) {
        // Check to see if minimal separator is a clique
        Vertex v = alpha_[i];
        Vertices S;
        for (Vertex u : G_->N(v)) {
            if (alpha_inverse_[v] < alpha_inverse_[u] && !is_deleted[u]) {
                S.add(u);
            }
        }
        for (Vertex u : F_[v]) {
            if (alpha_inverse_[v] < alpha_inverse_[u] && !is_deleted[u]) {
                S.add(u);
            }
        }
        if (G_->HasClique(S)) {
            for (Vertex u : deleted_vertices) {
                S.add(u);
            }
            cc.Separate(S);
            Vertices C = cc.ConnectedComponent(v);
            Vertices atom;
            for (Vertex u : C) {
                if (!is_deleted[u]) {
                    deleted_vertices.add(u);
                    is_deleted[u] = true;
                    atom.add(u);
                }
            }
            for (Vertex u : S) {
                if (!is_deleted[u]) {
                    atom.add(u);
                }
            }
            vertices_of_atoms.push_back(atom);
        }
    }
    // Remaining vertices form an atom
    Vertices C;
    for (Vertex v : *G_) {
        if (!is_deleted[v]) {
            C.add(v);
        }
    }
    if (!C.empty()) {
        vertices_of_atoms.push_back(C);
    }
    for (Vertices U : vertices_of_atoms) {
        atom_subgraphs_.push_back(new Graph(*G_, U));
    }
    return;
}

}  // namespace chordalg
