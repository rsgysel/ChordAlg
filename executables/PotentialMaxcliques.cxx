/*
 *  PotentialMaxcliques - Computes Potential Maximal Cliques
 *  of a graph
 *
 *  Warning: this computation requires exponential time/space
 *  in general
 */

#include <iostream>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/potential_maxcliques.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    for (auto a : *A) {
        PMCTrie* P = PotentialMaxcliques::Generate(*a);
        std::cout << a->str(*P) << '\n';
        delete P;
    }
    delete A;
    delete G;
    return EXIT_SUCCESS;
}
