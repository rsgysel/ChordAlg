/*
 *  separator_graph
 *
 *  Checks that the separator graph computed from the entire graph
 *  has the same number of edges as the sum of the edges from the
 *  separator graphs of the atom subgraphs
 */

#include <cassert>
#include <string>

#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + " <filename>";
    if (argc != 2) {
        std::cerr << usage << '\n';
        exit(EXIT_FAILURE);
    }
    std::cout << "Computing separator graph of cell intersection graph...";
    CellIntersectionGraph* G = CellIntersectionGraph::New(argv[1]);
    SeparatorGraph* SG = SeparatorGraph::New(G);
    std::cout << "done.\nComputing separator graph of partition intersection graph...";
    PartitionIntersectionGraph* H = PartitionIntersectionGraph::New(argv[1]);
    SeparatorGraph* SH = SeparatorGraph::New(H);
    std::cout << "done." << std::endl;
    assert(SG->size() == SH->size());
    assert(SG->order() == SH->order());
    std::cout << "Check successfully completed: " << SG->size()
              << " edges (crossing relations) and\n"
              << SG->order() << " vertices (minimal separators) found.\n";
    return(EXIT_SUCCESS);
}
