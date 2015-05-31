/*
 *  BTTreewidth - computes the treewidth of a graph using
 *  minimal separators and potential maxcliques
 */

#include <iostream>
#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/bt_algorithm.h"
#include "ChordAlgSrc/graph.h"
#include "chordalg_options.cpp"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    unsigned long long treewidth = 0;
    for (auto a : *A) {
        treewidth =
            std::max(treewidth, 
                     BTAlgorithm::Run(*a, BTAlgorithm::TreeWidth()));
    }
    std::cout << filename << " has treewidth " << treewidth << '\n';
    delete A;
    delete G;
    return EXIT_SUCCESS;
}
