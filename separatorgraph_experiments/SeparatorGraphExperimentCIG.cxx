/*
 *  SeparatorGraphExperimentCIG - computes the separator graph of a
 *  cell intersection graph
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/minimal_separator_algorithms.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        CellIntGraphFR* fr = 
            NewFileReader< CellIntGraphFR >(argv[1]);
        CellIntersectionGraph G(fr);
        SeparatorGraph SepG(&G, MinimalSeparators(G));
        std::cout << "Minimal separators: " << SepG.order() << '\n';
        std::cout << "Crossing relations: " << SepG.size() << '\n';
    }
    return EXIT_SUCCESS;
}
