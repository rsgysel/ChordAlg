/*
 *  ChordalGraph -  checks if a graph is chordal
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <graph filename>"
                  << " <elimination order filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        Graph* G = Graph::New(argv[1]);
        EliminationOrder* eo = EliminationOrder::FromFile(G, argv[2]);
        if (eo->IsPerfect()) {
            std::cout << "Elimination ordering is perfect.\n";
        } else {
            std::cout << "Elimination ordering is not perfect.\n";
        }
        delete eo;
        delete G;
        return EXIT_SUCCESS;
    }
}
