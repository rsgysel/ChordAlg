#include "graph_test.h"

// A simple test on a graph with two disjoint cliques. Intended to test the file reader, sorting / no self-loops of neighborhoods.
TEST_F(NexusGraphTest, NexusFileReaderTest) {
    Read(graph_dir() + std::string("test.nexus"));
    EXPECT_EQ(H->order(), 4);
    EXPECT_EQ(H->size(), 3);
    EXPECT_EQ(H->taxon_name(0), "taxon.name_1");
    EXPECT_EQ(H->taxon_name(1), "taxon.name_2");
    EXPECT_EQ(H->taxon_name(2), "taxon.name_3");
    return;
}

