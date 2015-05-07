/*
 *  CellIntersectionGraph - main program that computes and prints
 *  the cell intersection graph for a set of phylogenetic characters
 */

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        CellIntersectionGraph* G = CellIntersectionGraph::New(argv[1]);
        std::cout << G->str();
        delete G;
        return EXIT_SUCCESS;
    }
}
