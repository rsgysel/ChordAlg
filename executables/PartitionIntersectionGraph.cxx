/*
 *  PartitionIntersectionGraph.cpp - main program that computes and prints
 *  the partition intersection graph for a set of phylogenetic characters
 */

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        PartitionIntGraphFR* graph_reader =
            NewFileReader<PartitionIntGraphFR>(argv[1]);
        PartitionIntersectionGraph G(graph_reader);
        std::cout << G.str();
        return EXIT_SUCCESS;
    }
}
