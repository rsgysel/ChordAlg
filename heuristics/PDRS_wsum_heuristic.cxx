/*
 *  PDRS_wsum_heuristic - main program that implements a
 *  lb-elimination heuristic for maximum character compatibility.
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
#include <cstring>

#include <string>

#include "atom_subgraphs.hpp"
#include "lb_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"

using namespace chordalg;

void PDRS_ratio_heuristic_usage(std::string program) {
    std::cout << "usage: " << program << " <filename> <sep_weight> [runs]";
    std::cout << std::endl;
    std::cout << "where ``sep_weight'' is a positive number or ``inf'' that ";
    std::cout << "weights" << std::endl;
    std::cout << "the optional ``runs'' indicates # of times the heuristic ";
    std::cout << "is run on each atom subgraph" << std::endl;
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        PDRS_ratio_heuristic_usage(argv[0]);
    } else {
        Weight d, s;
        if (strncmp(argv[2], "inf", 3) == 0) {
            d = 0;
            s = 1;
        } else {
            d = 1;
            s = std::strtod(argv[2], NULL);
        }
        int runs = 0;
        if (argc == 4) {
            runs = std::strtod(argv[3], NULL);
        }
        std::cout << d << "*Deficiency(v) + " << s << "*Separation(v)";
        std::cout << std::endl;
        RunAtomHeuristic<ColoredIntersectionGraph,
                         MatrixCellIntGraphFR,
                         LBElimination,
                         WSumCriterion >
                         (argv[1], new WSumCriterion(d, s), runs);
    }
    return 1;
}
