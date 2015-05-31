/*
 *  minfill
 *
 *  Checks that the minfill of the
 *  connected components of a graph is equal to
 *  to the minfill of its atom subgraphs
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
    unsigned long long cc_minfill = 0;
    std::cout << "There are " << S.size() << " connected component(s).\n";
    std::cout << "Computing minfill of connected components...";
    for (Block B : S) {
        InducedSubgraph* H = InducedSubgraph::New(G, B.C());
        cc_minfill += BTAlgorithm::Run(*H, BTAlgorithm::WeightedMinimumFill());
        delete H;
    }
    std::cout << "done.\nComputing atoms...";
    Atoms* A = Atoms::New(G);
    std::cout << "done.\nThere are " << A->size() << " atom subgraph(s).\n";
    std::cout << "Computing minfill of atom subgraphs...";
    unsigned long long atoms_minfill = 0;
    for (auto a : *A) {
        atoms_minfill += BTAlgorithm::Run(*a, BTAlgorithm::WeightedMinimumFill());
    }
    std::cout << "done.\n" << std::endl;
    assert(cc_minfill == atoms_minfill);
    delete A;
    delete G;
    std::cout << "Check successfully completed: graph has minfill " << cc_minfill << '\n';
    return(EXIT_SUCCESS);
}
