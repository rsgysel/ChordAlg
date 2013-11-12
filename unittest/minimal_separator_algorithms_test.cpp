#include "chordalg_types.h"
#include "graph_test.h"
#include "minimal_separator_algorithms.h"

// Tests connected component calculation
TEST_F(SortedAdjacencyListTest, MinimalSeparators) {
    Read(graph_dir() + std::string("cc_test.sal"));

}
