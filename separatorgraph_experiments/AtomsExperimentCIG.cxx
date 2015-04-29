/*
 *  AtomsExperiment.cpp - computes the atom subgraphs of a cell
 *  intersection graph
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        CellIntGraphFR* graph_reader =
            NewFileReader<CellIntGraphFR>(
                std::string(argv[1]));
        Graph G(graph_reader);
        Atoms A(&G);
        A.ComputeAtoms();
        size_t vertices = 0, edges = 0;
        for (auto a : A) {
            vertices += a->order();
            edges += a->size();
        }
        std::cout << "Vertices: " << G.order() << '\n'
                  << "Edges: " << G.size() << '\n'
                  << "Total Vertices: " << vertices << '\n'
                  << "Total Edges: " << edges << '\n'
                  << "Total Atoms: " << A.size() << '\n'
                  << "Clique Atoms: " << A.clique_minimal_separators().size() << std::endl;
        return EXIT_SUCCESS;
    }
}
