/*
 *  BTTreewidth - computes the minimum_fill of a graph using
 *  minimal separators and potential maxcliques
 *
 *  Warning: this computation requires exponential time/space
 *  in general
 */

#include <iostream>
#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/bt_algorithm.h"
#include "ChordAlgSrc/graph.h"
#include "chordalg_options.cpp"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string filename;
    ChordAlgOptions(argc, argv, &filename);
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    unsigned long long minimum_fill = 0;
    for (auto a : *A) {
        minimum_fill = 
            std::max(minimum_fill,
                     BTAlgorithm::Run(*a, BTAlgorithm::WeightedMinimumFill()));
    }
    std::cout << filename << " has minimum_fill " << minimum_fill << '\n';
    delete A;
    delete G;
    return EXIT_SUCCESS;
}
