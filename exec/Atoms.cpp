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
#include <string>
#include <sstream>

#include "atom_subgraphs.hpp"
#include "file_reader.h"
#include "intersection_graph.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    } else {
        chordalg::MatrixCellIntGraphFR* graph_reader =
            chordalg::NewFileReader<chordalg::MatrixCellIntGraphFR>(
                std::string(argv[1]));
        chordalg::ColoredIntersectionGraph G(graph_reader);
        chordalg::Atoms< chordalg::ColoredIntersectionGraph > A(G);
        int i = 0;
        for (chordalg::ColoredIntersectionGraph* a : A) {
            std::stringstream ss;
            ss << i;
            std::string suffix;
            if (a->IsClique()) {
                suffix = std::string("_cliqueatom");
            } else {
                suffix = std::string("_atom");
            }
            std::string atom_filename = std::string(argv[1]) + suffix +
                                        ss.str();
            a->Print(atom_filename);
            ++i;
        }
    }
    return 0;
}
