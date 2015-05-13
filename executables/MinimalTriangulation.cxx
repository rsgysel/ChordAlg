/*
 *  MinimalTriangulation - checks if triangulation is a minimal
 *  triangulation of the specified graph
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string graph_filename, triangulation_filename;
    ChordAlgOptions(argc, argv, &graph_filename, nullptr, &triangulation_filename);
    Graph* G = Graph::New(graph_filename);
    Triangulation* T = Triangulation::New(G, triangulation_filename);
    if (!T->IsSupergraph(G)) {
        std::cerr << "Error: " << triangulation_filename << " is not a supergraph of ";
        std::cerr << graph_filename << '\n';
        exit(EXIT_FAILURE);
    } else if (T->IsMinimalTriangulation()) {
        std::cout << "Triangulation is minimal.\n";
    } else {
        std::cout << "Triangulation is not minimal.\n";
    }
    delete T;
    delete G;
    return EXIT_SUCCESS;
}
