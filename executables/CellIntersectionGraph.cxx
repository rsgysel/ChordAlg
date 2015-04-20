/*
 *  CellIntersectionGraph.cpp - main program that computes and prints
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
        MatrixCellIntGraphFR* graph_reader =
            NewFileReader<MatrixCellIntGraphFR>(argv[1]);
        ColoredIntersectionGraph G(graph_reader);
        std::cout << G.str();
        return EXIT_SUCCESS;
    }
}
