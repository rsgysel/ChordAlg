/*
 *  treewidth
 *
 *  Checks that the maximum treewidth of the
 *  connected components of a graph is equal to
 *  to the maximum treewidth of its atom subgraphs
 */

#include <cassert>
#include <limits>
#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/bt_algorithm.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + " <filename>";
    if (argc != 2) {
        std::cerr << usage << '\n';
        exit(EXIT_FAILURE);
    }
    Graph* G = Graph::New(argv[1]);
    SeparatorBlocks S(G);
    S.Separate(Vertices());
    unsigned long long cc_tw = 0;
    std::cout << "There are " << S.size() << " connected component(s).\n";
    std::cout << "Computing treewidth of connected components...";
    for (Block B : S) {
        InducedSubgraph* H = InducedSubgraph::New(G, B.C());
        cc_tw = std::max(cc_tw, BTAlgorithm::Run(*H, BTAlgorithm::TreeWidth()));
        delete H;
    }
    std::cout << "done.\nComputing atoms...";
    Atoms* A = Atoms::New(G);
    std::cout << "done.\nThere are " << A->size() << " atom subgraph(s).\n";
    std::cout << "Computing treewidth of atom subgraphs...";
    unsigned long long atoms_tw = 0;
    for (auto a : *A) {
        atoms_tw = std::max(atoms_tw, BTAlgorithm::Run(*a, BTAlgorithm::TreeWidth()));
    }
    std::cout << "done.\n" << std::endl;
    assert(cc_tw == atoms_tw);
    delete A;
    delete G;
    std::cout << "Check successfully completed: graph has treewidth " << cc_tw << '\n';
    return(EXIT_SUCCESS);
}
