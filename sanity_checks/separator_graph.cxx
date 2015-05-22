/*
 *  separator_graph
 *
 *  Checks that the separator graph computed from the entire graph
 *  has the same number of edges as the sum of the edges from the
 *  separator graphs of the atom subgraphs
 */

#include <cassert>
#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + " <filename>";
    if (argc != 2) {
        std::cerr << usage << '\n';
        exit(EXIT_FAILURE);
    }
    Graph* G = Graph::New(argv[1]);
    std::cout << "Computing separator graph...";
    SeparatorGraph* SG = SeparatorGraph::New(G);
    std::cout << "done.\nComputing atoms...";
    Atoms* A = Atoms::New(G);
    std::cout << "done.\nThere are " << A->size() << " atoms subgraph(s).\n";
    std::cout << "Computing separator graphs of atoms...";
    size_t total_edges = 0;
    for (auto a : *A) {
        SeparatorGraph* SGa = SeparatorGraph::New(a);
        total_edges += SGa->size();
        delete SGa;
    }
    std::cout << "done.\n";
    assert(SG->size() == total_edges);
    delete A;
    delete SG;
    delete G;
    std::cout << "Check successfully completed: " << total_edges
              << " edges (crossing relations) found.\n";
    return(EXIT_SUCCESS);
}
