/*
 *  AtomSeparatorGraphExperimentsPIG - computes the separator graph of
 *  a partition intersection graph using its atom subgraphs
 */

#include <cstring>

#include <string>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/minimal_separator_algorithms.h"
#include "ChordAlgSrc/separator_graph.h"

using namespace chordalg;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <filename>";
        return EXIT_FAILURE;
    } else {
        PartitionIntersectionGraph* G = PartitionIntersectionGraph::New(argv[1]);
        Atoms* A = Atoms::New(G);
        size_t minimal_separators = A->clique_minimal_separators().size();
        size_t crossing_relations = 0;
        for (auto a : *A) {
            SeparatorGraph* SepG = SeparatorGraph::New(a);
            minimal_separators += SepG->order();
            crossing_relations += SepG->size();
            delete SepG;
        }

        std::cout << "Minimal separators: " << minimal_separators << '\n';
        std::cout << "Crossing relations: " << crossing_relations << '\n';
        delete A;
        delete G;
        return EXIT_SUCCESS;
    }
}
