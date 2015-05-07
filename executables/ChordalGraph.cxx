/*
 *  ChordalGraph - main program that checks if a graph is chordal
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        Graph* G = Graph::New(argv[1]);
        if (Triangulation::IsChordal(G)) {
            std::cout << "Graph is chordal.\n";
        } else {
            std::cout << "Graph is not chordal.\n";
        }
        delete G;
        return EXIT_SUCCESS;
    }
}
