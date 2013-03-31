#include "graph_test.h"

TEST_F(MatrixCellIntGraphTest, FileReaderTest) {

    Read(graph_dir() + std::string("cig_test.m"));
    chordalg::Graph H( chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >( graph_dir() + std::string("cig_test.sal") ) );
    EXPECT_EQ(G->IsIsomorphic(H),true);

    return;
}
