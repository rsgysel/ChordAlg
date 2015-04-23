/*
 *  AtomsExperiment.cpp - main program that computes and prints the total
 *  vertices and edges of the atom subgraphs of a graph
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
                  << "Total Edges: " << edges << std::endl;
        return EXIT_SUCCESS;
    }
}
