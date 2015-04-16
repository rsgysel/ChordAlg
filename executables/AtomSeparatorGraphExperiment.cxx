/*
 *  AtomSeparatorGraphExperiments - main program that computes the 
 *  separator graph of each atom and print the total number of
 *  minimal separators and crossing relations of a graph
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

#include <cstring>

#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/minimal_separator_algorithms.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

void SeparatorGraph_usage(std::string program) {
    std::cout << "usage: " << program << " <filename>";
    std::cout << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        SeparatorGraph_usage(argv[0]);
        return EXIT_FAILURE;
    } else {
        chordalg::MatrixCellIntGraphFR* fr = 
            chordalg::NewFileReader< chordalg::MatrixCellIntGraphFR >(argv[1]);
        chordalg::ColoredIntersectionGraph G(fr);

        chordalg::Atoms A(&G);
        A.ComputeAtoms();
        size_t minimal_separators = A.clique_minimal_separators().size(), crossing_relations = 0;
        for (auto a : A) {
            chordalg::SeparatorGraph SepG(a, chordalg::MinimalSeparators(*a));
            minimal_separators += SepG.order();
            crossing_relations += SepG.size();
        }

        std::cout << "Minimal separators: " << minimal_separators << '\n';
        std::cout << "Crossing relations: " << crossing_relations << '\n';
    }
    return EXIT_SUCCESS;
}
