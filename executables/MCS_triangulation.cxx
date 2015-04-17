#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/mcs.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        SortedAdjacencyListFR* fr =
            NewFileReader< SortedAdjacencyListFR >(argv[1]);
        Graph G(fr);
        EliminationOrder* eo = MCS(G);
        eo->ComputeFill();
        Triangulation H(&G, eo);
        std::cout << H.str();
        return EXIT_SUCCESS;
    }
}
