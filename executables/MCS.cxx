/*
 *  MCS - runs Maximum Cardinality Search to obtain an elimination
 *  ordering of a graph. If graph is chordal, elimination ordering
 *  is perfect.
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    EliminationOrder* eo = MCS(G);
    std::cout << eo->str();
    delete eo;
    delete G;
    return EXIT_SUCCESS;
}
