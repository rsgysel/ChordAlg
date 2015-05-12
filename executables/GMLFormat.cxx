/*
 *  GMLFormat - outputs GML format of a graph
 */

#include <iostream>

#include "ChordAlgSrc/chordalg_string.h"
#include "ChordAlgSrc/graph.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        Graph* G = Graph::New(argv[1]);
        StringTokens tokens = Split(argv[1], "./");
        std::cout << G->strGML(tokens[tokens.size() - 2]);
        delete G;
        return EXIT_SUCCESS;
    }
}
