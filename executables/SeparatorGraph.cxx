/*
 *  SeparatorGraph - computes separator graph of a graph
 */

#include <iostream>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator_graph.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    SeparatorGraph* SG = SeparatorGraph::New(G);
    std::cout << SG->str();
    delete SG;
    delete G;
    return EXIT_SUCCESS;
}
