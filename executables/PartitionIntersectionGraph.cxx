/*
 *  PartitionIntersectionGraph -  computes and prints
 *  the partition intersection graph for a set of phylogenetic characters
 */

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename, nullptr, nullptr, FileMode::CHARACTERMATRIX);
    PartitionIntersectionGraph* G = PartitionIntersectionGraph::New(filename);
    std::cout << G->str();
    delete G;
    return EXIT_SUCCESS;
}
