/*
 *  FillEliminationOrder - computes triangulation of a graph
 *  determined by filling an elimination order
 */

#include <iostream>

#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/triangulation.h"
#include "chordalg_options.cpp"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string graph_filename, eo_filename;
    ChordAlgOptions(argc, argv, &graph_filename, &eo_filename);
    Graph* G = Graph::New(graph_filename);
    EliminationOrder* eo = EliminationOrder::File(G, eo_filename);
    Triangulation* T = Triangulation::New(G, *eo);
    std::cout << T->str();
    delete T;
    delete eo;
    delete G;
    return EXIT_SUCCESS;
}
