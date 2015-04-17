/*
 *  Atoms.cpp - main program that computes and prints the atom subgraphs
 *  of a graph
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
