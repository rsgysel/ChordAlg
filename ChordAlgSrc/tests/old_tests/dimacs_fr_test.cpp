#include "graph_test.h"

// A simple test on a graph with two disjoint cliques. Intended to test the file reader, sorting / no self-loops of neighborhoods.
TEST_F(DimacsGraphTest, DimacsFileReaderTest) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    //G->PrettyPrint();
    return;
}
