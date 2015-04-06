#include "graph_test.h"

#include "mcs.h"
#include "tree_representation.h"

// A simple test on a graph with two disjoint cliques. Intended to test the file reader, sorting / no self-loops of neighborhoods.
TEST_F(NexusGraphTest, NexusFileReaderTest) {
    Read(graph_dir() + std::string("test.nexus"));
    EXPECT_EQ(H->order(), 4);
    EXPECT_EQ(H->size(), 3);
    EXPECT_EQ(H->taxon_name(0), "taxon.name_1"  );
    EXPECT_EQ(H->taxon_name(1), "taxon.name_2"  );
    EXPECT_EQ(H->taxon_name(2), "roottaxon"     );
    return;
}

TEST_F(NexusGraphTest, NexusRootedPhyloTest) {
    Read(graph_dir() + std::string("test.nexus"));
    chordalg::CliqueTree* ct = chordalg::MCSCliqueTree(*H);

    std::stringstream phylo_buffer;
    std::streambuf* old = std::cout.rdbuf(phylo_buffer.rdbuf());
    ct->PhyloNewickPrint(*H, true);
    std::cout.rdbuf( old );
    std::stringstream check_phylo;
    old = std::cout.rdbuf(check_phylo.rdbuf());
    // These strings have been carefully hand-checked, see jpgs in graphfiles
    std::cout << "((taxon.name_2,(taxon.name_1)))roottaxon;" << std::endl;
    std::cout.rdbuf( old );
    delete ct;
    return;
}
