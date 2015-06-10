/*
 *  BTTreewidth - computes the treewidth of a graph using
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
    unsigned long long treewidth =
        BTScheme::Run(filename, BTScheme::TreeWidth());
    std::cout << filename << " has treewidth " << treewidth << '\n';
    return EXIT_SUCCESS;
}
