#include "chordalg_types.h"
#include "graph_test.h"
#include "algorithms.h"

// Tests connected component calculation
TEST_F(SortedAdjacencyListTest, MinimalSeparators) {
    Read(graph_dir() + std::string("cc_test.sal"));

}
