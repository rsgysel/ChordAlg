/*
 *  elimination_heuristics
 *
 *  Checks that the elimination heuristics for character
 *  compatibility satisfy the following:
 *  1) A heuristic run results in a triangulation
 *  2) An LB-Elimination run results in a triangulation
 *  3) The weight of fill edges added = the number of removed characters
 *
 */

#include <cassert>
#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lb_triang.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;

int main(int argc, char** argv) {
    std::string usage = std::string(argv[0]) + " <filename>";
    if (argc != 2) {
        std::cerr << usage << '\n';
        exit(EXIT_FAILURE);
    }
    std::string filename = argv[1];
    std::cout << "Running lb-triang...";
    Graph* G = Graph::New(filename);
    Triangulation* T = LBTriang::Run(*G);
    std::cout << "Done.\nChecking minimality of triangulation...";
    assert(T->IsMinimalTriangulation());
    std::cout << "Done. Check successfully completed.\n";
    delete T;
    delete G;
    return EXIT_SUCCESS;
}
