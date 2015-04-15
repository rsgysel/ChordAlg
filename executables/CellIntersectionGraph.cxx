/*
 *  CellIntersectionGraph.cpp - main program that computes and prints
 *  the cell intersection graph for a set of phylogenetic characters
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

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    } else {
        MatrixCellIntGraphFR* graph_reader =
            NewFileReader<MatrixCellIntGraphFR>(argv[1]);
        ColoredIntersectionGraph G(graph_reader);
        std::cout << "vertices: " << G.order() << std::endl;
        std::cout << "edges: " << G.size() << std::endl;
    }
}
