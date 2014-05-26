#include "chordalg_types.h"
#include "graph_test.h"
#include "minimal_separator_algorithms.h"

TEST_F(SortedAdjacencyListTest, MinsepsOfClique) {
    Read(graph_dir() + std::string("clique.sal"));
    chordalg::LexTrie* minseps = chordalg::BerryBordatCogis(*G);
    EXPECT_EQ(minseps->Size(), 0);
}

TEST_F(SortedAdjacencyListTest, MinsepsOfCliques) {
    Read(graph_dir() + std::string("twocliques.sal"));
    chordalg::LexTrie* minseps = chordalg::BerryBordatCogis(*G);
    EXPECT_EQ(minseps->Size(), 1); // Empty set is minsep
}

TEST_F(SortedAdjacencyListTest, MinsepsOfTree) {
    Read(graph_dir() + std::string("tree.sal"));
    chordalg::LexTrie* minseps = chordalg::BerryBordatCogis(*G);
    EXPECT_EQ(minseps->Size(), 6); // Six internal nodes
}

// G(B) = tack s and t on to bipartite graph B
TEST_F(SortedAdjacencyListTest, BipartiteReduction) {
    Read(graph_dir() + std::string("bipartite_reduction.sal"));
    chordalg::LexTrie* ab_minseps = chordalg::KloksKratsch(*G, 6, 7);
    EXPECT_EQ(ab_minseps->Size(), 4);
    chordalg::LexTrie* minseps = chordalg::BerryBordatCogis(*G);
    EXPECT_EQ(minseps->Size(), 9);
}

TEST_F(SortedAdjacencyListTest, BipartiteReductionSTMinsepComponents) {
    Read(graph_dir() + std::string("bipartite_reduction.sal"));
    chordalg::LexTrie* ab_minseps = chordalg::KloksKratsch(*G, 6, 7);
    for(chordalg::Subset S : *ab_minseps)
    {
        chordalg::SeparatorBlocks B(*G);
        B.Separate(S);
        EXPECT_EQ(B.FullComponentCt(), 2);
        EXPECT_EQ(B.NonFullComponentCt(),0);
    }
}

// G'(B) = add x, sx, tx to G(B)
TEST_F(SortedAdjacencyListTest, XBipartiteReduction) {
    Read(graph_dir() + std::string("bipartite_reduction_x.sal"));
    chordalg::LexTrie* ab_minseps = chordalg::KloksKratsch(*G, 6, 7);
    EXPECT_EQ(ab_minseps->Size(), 4);
}

TEST_F(SortedAdjacencyListTest, XBipartiteReductionSTMinsepComponents) {
    Read(graph_dir() + std::string("bipartite_reduction_x.sal"));
    chordalg::LexTrie* ab_minseps = chordalg::KloksKratsch(*G, 6, 7);
    for(chordalg::Subset S : *ab_minseps)
    {
        chordalg::SeparatorBlocks B(*G);
        B.Separate(S);
        EXPECT_EQ(B.FullComponentCt(), 2);
        EXPECT_EQ(B.NonFullComponentCt(),0);
    }
}

// G'(B)_{s,t}
TEST_F(SortedAdjacencyListTest, STBipartiteReduction) {
    Read(graph_dir() + std::string("bipartite_reduction_st.sal"));
    chordalg::LexTrie* ab_minseps = chordalg::KloksKratsch(*G, 6, 7);
    EXPECT_EQ(ab_minseps->Size(), 0);
    chordalg::LexTrie* minseps = chordalg::BerryBordatCogis(*G);
    EXPECT_EQ(minseps->Size(), 8);
}
