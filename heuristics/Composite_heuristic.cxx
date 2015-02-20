/*
 *  Composite_herusitic.cpp - main program that implements classic and
 *  lb-elimination heuristics for maximum character compatibility
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

#include <algorithm>
#include <string>

#include "atom_subgraphs.hpp"
#include "classic_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "lb_elimination.h"

using namespace chordalg;

void Composite_heuristic_usage(std::string program) {
    std::cout << "usage: " << program << " <filename> <sep_weight>";
    std::cout << std::endl;
    std::cout << "where ``sep_weight'' is a real number or ``inf''";
    std::cout << std::endl;
    return;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        Composite_heuristic_usage(argv[0]);
    } else {
        Weight d, s;
        if (strncmp(argv[2], "inf", 3) == 0) {
            d = 0;
            s = 1;
        } else {
            d = 1;
            s = std::strtod(argv[2], NULL);
        }

        std::cout << d << "*Deficiency(v) + " << s << "*Separation(v)";
        std::cout << std::endl;
        MatrixCellIntGraphFR* graph_reader =
            NewFileReader<MatrixCellIntGraphFR>(argv[1]);
        ColoredIntersectionGraph G(graph_reader);
        Weight total_weight = 0;
        Atoms< ColoredIntersectionGraph > A(G);
        int clique_atoms = 0, atom_id = 0, total_count = 0;
        for (ColoredIntersectionGraph* a : A) {
            ++atom_id;
            if (!a->IsClique()) {
                LBElimination eo1(*a, new RatioCriterion());
                LBElimination eo2(*a, new WSumCriterion( d, s ));
                ClassicElimination eo3(*a, new ClassicCriterion());
                Weight min_weight = std::min(eo1.fill_cost(),
                                std::min(eo2.fill_cost(), eo3.fill_cost()));
                if (eo1.fill_cost() == min_weight) {
                    total_weight += eo1.fill_cost();
                    total_count += eo1.fill_count();
                    std::cout << "atom (LB, Ratio)" << atom_id << std::endl;
                    eo1.PrettyPrint();
                } else if (eo2.fill_cost() == min_weight) {
                    total_weight += eo2.fill_cost();
                    total_count += eo2.fill_count();
                    std::cout << "atom (LB, Weighted Sum)" << atom_id;
                    std::cout << std::endl;
                    eo2.PrettyPrint();
                } else if (eo3.fill_cost() == min_weight) {
                    total_weight +=  eo3.fill_cost();
                    total_count += eo3.fill_count();
                    std::cout << "atom (Classic)" << atom_id << std::endl;
                    eo3.PrettyPrint();
                }
            } else {
                ++clique_atoms;
            }
        }
        std::cout << "fill weight: " << total_weight << std::endl;
        std::cout << "fill count: " << total_count << std::endl;
        std::cout << "vertices: " << G.order() << std::endl;
        std::cout << "edges: " << G.size() << std::endl;
        std::cout << "atoms: " << A.size() << std::endl;
        std::cout << "clique atoms: " << clique_atoms << std::endl;
    }
    return 0;
}
