/*
 *  MMC_MaxCompatSupertree -  constructs a supertree
 *  from an MRP matrix representing a collection of partially labeled
 *  trees. Utilizes MMC heuristic to estimate maximum compatibility.
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/mcs.h"
#include "ChordAlgSrc/tree_representation.h"

using namespace chordalg;

void MMC_heuristic_usage(std::string program) {
    std::cerr << "usage: " << program << " <filename>" << std::endl;
    std::cerr << "where filename is a MRP matrix file in NEXUS format";
    std::cerr << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        MMC_heuristic_usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        CellIntersectionGraph* G = CellIntersectionGraph::New(argv[1]);
        ClassicElimination eo(G, new DeficiencyCriterion());
        Supergraph triangulation(G, eo.TriangNbhds());
        CliqueTree* ct = MCS::NewCliqueTree(triangulation);
        std::cout << ct->strPhyloNewick(*G, true) << std::endl << std::endl;
        std::cout << eo.str() << std::endl;
        std::cout << "fill weight: " << eo.fill_cost() << std::endl;
        std::cout << "fill count: " << eo.fill_count() << std::endl;
        std::cout << "vertices: " << G->order() << std::endl;
        std::cout << "edges: " << G->size() << std::endl;
        delete ct;
	delete G;
        return EXIT_SUCCESS;
    }
}

