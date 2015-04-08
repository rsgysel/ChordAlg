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

// The integration tests in this file are brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// how the file reader parses a graph file.

// Compares the adjacency list to the Dimacs format representation
TEST(DimacsGraphFRTest, Integration) {
    MockGraphFile mock_file;
    std::ifstream line_stream, eof_marker;
    eof_marker.peek(), eof_marker.peek();  // Twice for end-of-file bit
    EXPECT_CALL(mock_file, GetLine(_))
        .Times(22)
        .WillOnce(GraphFileInputLine("p edge 11 20", line_stream))
        .WillOnce(GraphFileInputLine("e 1 2", line_stream))
        .WillOnce(GraphFileInputLine("e 1 4", line_stream))
        .WillOnce(GraphFileInputLine("e 1 7", line_stream))
        .WillOnce(GraphFileInputLine("e 1 9", line_stream))
        .WillOnce(GraphFileInputLine("e 2 3", line_stream))
        .WillOnce(GraphFileInputLine("e 2 6", line_stream))
        .WillOnce(GraphFileInputLine("e 2 8", line_stream))
        .WillOnce(GraphFileInputLine("e 3 5", line_stream))
        .WillOnce(GraphFileInputLine("e 3 7", line_stream))
        .WillOnce(GraphFileInputLine("e 3 10", line_stream))
        .WillOnce(GraphFileInputLine("e 4 5", line_stream))
        .WillOnce(GraphFileInputLine("e 4 6", line_stream))
        .WillOnce(GraphFileInputLine("e 4 10", line_stream))
        .WillOnce(GraphFileInputLine("e 5 8", line_stream))
        .WillOnce(GraphFileInputLine("e 5 9", line_stream))
        .WillOnce(GraphFileInputLine("e 6 11", line_stream))
        .WillOnce(GraphFileInputLine("e 7 11", line_stream))
        .WillOnce(GraphFileInputLine("e 8 11", line_stream))
        .WillOnce(GraphFileInputLine("e 9 11", line_stream))
        .WillOnce(GraphFileInputLine("e 10 11", line_stream))
        .WillOnce(ReturnRef(eof_marker));
    chordalg::DimacsGraphFR* file_reader =
        chordalg::NewFileReader< chordalg::DimacsGraphFR >(mock_file);
    chordalg::Graph G(file_reader);
    delete file_reader;
    // Construct graph from adjacency list
    chordalg::Graph H(new chordalg::AdjacencyLists(dimacs_test));
    // Compare
    EXPECT_EQ(G.IsIsomorphic(H), true);
}

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

// Compares the adjacency list to the matrix representation
TEST(NexusMRPFRTest, Integration) {
    // Construct graph from matrix representation
    MockGraphFile mock_file;
    std::ifstream line_stream, eof_marker;
    eof_marker.peek(), eof_marker.peek();  // Twice for end-of-file bit
    EXPECT_CALL(mock_file, GetLine(_))
        .Times(11)
        .WillOnce(GraphFileInputLine("#NEXUS", line_stream))
        .WillOnce(GraphFileInputLine("Begin Data", line_stream))
        .WillOnce(GraphFileInputLine("Dimensions ntax=3 nchar=3", line_stream))
        .WillOnce(GraphFileInputLine("Format datatype=standard symbols=\"01\" Missing=?", line_stream))
        .WillOnce(GraphFileInputLine("Matrix", line_stream))
        .WillOnce(GraphFileInputLine("taxon1 000", line_stream))
        .WillOnce(GraphFileInputLine("taxon2 010", line_stream))
        .WillOnce(GraphFileInputLine("root ?01", line_stream))
        .WillOnce(GraphFileInputLine(";", line_stream))
        .WillOnce(GraphFileInputLine("End;", line_stream))
        .WillOnce(ReturnRef(eof_marker));
    chordalg::NexusMRPFR* file_reader =
        chordalg::NewFileReader< chordalg::NexusMRPFR >(mock_file);
    chordalg::Graph CIG(file_reader);
    delete file_reader;
    // Construct graph from adjacency list
    chordalg::Graph G(new chordalg::AdjacencyLists(nexus_test));
    // Compare
    EXPECT_EQ(G.IsIsomorphic(CIG), true);
}

