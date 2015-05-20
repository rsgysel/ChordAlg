/*
 *  PerfectEliminationOrder - checks if an elimination ordering
 *  is perfect (zero fill)
 */

#include <iostream>

#include "ChordAlgSrc/elimination.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string graph_filename, eo_filename;
    ChordAlgOptions(argc, argv, &graph_filename, &eo_filename);
    Graph* G = Graph::New(graph_filename);
    EliminationOrder* eo = EliminationOrder::File(G, eo_filename);
    if (Elimination::ZeroFill(*G, *eo)) {
        std::cout << "Elimination ordering is perfect.\n";
    } else {
        std::cout << "Elimination ordering is not perfect.\n";
    }
    delete eo;
    delete G;
    return EXIT_SUCCESS;
}
