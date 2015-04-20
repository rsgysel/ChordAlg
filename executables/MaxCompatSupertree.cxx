/*
 *  MMC_MaxCompatSupertree - main program that constructs a supertree
 *  from an MRP matrix representing a collection of partially labeled
 *  trees. Utilizes MMC heuristic to estimate maximum compatibility.
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. �See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. �If not, see <http://www.gnu.org/licenses/>.
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
        NexusMRPFR* graph_reader = NewFileReader<NexusMRPFR>(argv[1]);
        ColoredIntersectionGraph G(graph_reader);
        ClassicElimination eo(&G, new DeficiencyCriterion());
        Supergraph triangulation(&G, eo.TriangNbhds());
        CliqueTree* ct = MCSCliqueTree(triangulation);
        std::cout << ct->strPhyloNewick(G, true) << std::endl << std::endl;
        std::cout << eo.str() << std::endl;
        std::cout << "fill weight: " << eo.fill_cost() << std::endl;
        std::cout << "fill count: " << eo.fill_count() << std::endl;
        std::cout << "vertices: " << G.order() << std::endl;
        std::cout << "edges: " << G.size() << std::endl;
        delete ct;
        return EXIT_SUCCESS;
    }
}
