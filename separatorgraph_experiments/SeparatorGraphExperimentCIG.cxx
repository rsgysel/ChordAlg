/*
 *  SeparatorGraphExperimentCIG - computes the separator graph of a
 *  cell intersection graph
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    } else {
        CellIntersectionGraph* G = CellIntersectionGraph::New(argv[1]);
        SeparatorGraph* SepG = SeparatorGraph::New(G);
        std::cout << "Minimal separators: " << SepG->order() << '\n';
        std::cout << "Crossing relations: " << SepG->size() << '\n';
        delete SepG;
        delete G;
    }
    return EXIT_SUCCESS;
}
