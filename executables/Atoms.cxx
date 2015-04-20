/*
 *  Atoms.cpp - main program that computes and prints the atom subgraphs
 *  of a graph
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"

using namespace chordalg;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        SortedAdjacencyListFR* graph_reader =
            NewFileReader<SortedAdjacencyListFR>(
                std::string(argv[1]));
        Graph G(graph_reader);
        Atoms A(&G);
        A.ComputeAtoms();
        size_t i = 0;
        for (auto a : A) {
            std::string suffix;
            if (a->IsClique()) {
                suffix = std::string("_cliqueatom");
            } else {
                suffix = std::string("_atom");
            }
            std::string atom_filename = std::string(argv[1]) + suffix +
                                        std::to_string(i);
            std::cout << "Creating file " << atom_filename << std::endl;
            std::ofstream atom_file;
            atom_file.open(atom_filename, std::ofstream::out);
            atom_file << a->str();
            atom_file.close();
            ++i;
        }
        return EXIT_SUCCESS;
    }
}
