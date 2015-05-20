/*
 *  CliqueTree - prints a clique tree of a chordal graph
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs.h"
#include "ChordAlgSrc/tree_representation.h"
#include "ChordAlgSrc/triangulation.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    Triangulation* H = Triangulation::New(G);
    CliqueTree* T = MCS::NewCliqueTree(*H);
    std::cout << T->str();
    delete T;
    delete H;
    delete G;
    return EXIT_SUCCESS;
}
