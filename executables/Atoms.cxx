/*
 *  Atoms -  computes and prints the atom subgraphs
 *  of a graph.
 *
 *  Unlike other executables in ChordAlg, Atoms
 *  will print to files instead of stdout.
 *  If graph.g is the input file, then atom subgraphs
 *  will look like either graph.g_atom9 or
 *  graph.g_cliqueatom5
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "chordalg_options.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    size_t i = 0;
    for (auto a : *A) {
        std::string suffix;
        if (a->IsClique()) {
            suffix = std::string("_cliqueatom");
        } else {
            suffix = std::string("_atom");
        }
        std::string atom_filename = filename + suffix +
                                    std::to_string(i);
        std::cout << "Creating file " << atom_filename << std::endl;
        std::ofstream atom_file;
        atom_file.open(atom_filename, std::ofstream::out);
        atom_file << a->str();
        atom_file.close();
        ++i;
    }
    delete A;
    delete G;
    return EXIT_SUCCESS;
}
