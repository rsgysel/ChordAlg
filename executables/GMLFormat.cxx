/*
 *  GMLFormat - outputs GML format of a graph
 */

#include <iostream>

#include "ChordAlgSrc/chordalg_string.h"
#include "ChordAlgSrc/graph.h"
#include "chordalg_options.cpp"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    StringTokens tokens = Split(filename, "./");
    std::cout << G->strGML(tokens[tokens.size() - 2]);
    delete G;
    return EXIT_SUCCESS;
}
