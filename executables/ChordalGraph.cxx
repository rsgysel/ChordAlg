/*
 *  ChordalGraph -  checks if a graph is chordal
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    if (Triangulation::IsTriangulated(G)) {
        std::cout << "Graph is chordal.\n";
    } else {
        std::cout << "Graph is not chordal.\n";
    }
    delete G;
    return EXIT_SUCCESS;
}
