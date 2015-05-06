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
        Graph* G = Graph::New(argv[1]);
        EliminationOrder* eo = MCS(*G);
        Triangulation* H = Triangulation::New(G, eo);
        std::cout << H->str();
        delete H;
	delete eo;
        delete G;
        return EXIT_SUCCESS;
    }
}
