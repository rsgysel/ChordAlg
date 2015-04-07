#include <fstream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "test_graphs.h"
#include "mock_graph_file.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

// Compares the adjacency list to the matrix representation
TEST(MatrixCellIntGraphFRTest, Integration) {
    // Construct graph from matrix representation
    MockGraphFile mock_file;
    std::ifstream line_stream, eof_marker;
    eof_marker.peek(), eof_marker.peek();  // Twice for end-of-file bit
    EXPECT_CALL(mock_file, GetLine(_))
        .Times(11)
        .WillOnce(GraphFileInputLine("id", line_stream))
        .WillOnce(GraphFileInputLine("8 6", line_stream))
        .WillOnce(GraphFileInputLine("? 0 ? 0 0 0", line_stream))
        .WillOnce(GraphFileInputLine("1 1 1 0 0 1", line_stream))
        .WillOnce(GraphFileInputLine("? ? 2 1 ? 2", line_stream))
        .WillOnce(GraphFileInputLine("3 1 1 0 0 ?", line_stream))
        .WillOnce(GraphFileInputLine("2 2 2 1 1 ?", line_stream))
        .WillOnce(GraphFileInputLine("3 1 1 0 ? ?", line_stream))
        .WillOnce(GraphFileInputLine("? 0 1 0 0 0", line_stream))
        .WillOnce(GraphFileInputLine("0 0 0 ? 0 0", line_stream))
        .WillOnce(ReturnRef(eof_marker));
    chordalg::MatrixCellIntGraphFR* file_reader =
        chordalg::NewFileReader< chordalg::MatrixCellIntGraphFR >(mock_file);
    chordalg::Graph CIG(file_reader);
    delete file_reader;
    // Construct graph from adjacency list
    chordalg::Graph G(new chordalg::AdjacencyLists(cig_test));
    // Compare
    EXPECT_EQ(G.IsIsomorphic(CIG), true);
}
