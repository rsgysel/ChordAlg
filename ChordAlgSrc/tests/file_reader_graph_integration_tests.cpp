#include <fstream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "test_graphs.h"
#include "mock_graph_file.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;


/////////////
// Frameworks

class FileReadingTest : public ::testing::Test {
 public:
    void SetUp() {
        eof_marker_.peek(); eof_marker_.peek(); // Twice for end-of-file bit
        mock_file_ = new MockGraphFile();
    }
    void TearDown() {
        delete mock_file_;
    }
 protected:
    std::ifstream line_stream_, eof_marker_;
    MockGraphFile* mock_file_;
};  // FileReadingTest

class GetLinesTest : public FileReadingTest {
 public:
    void SetUp() {
        file_reader_ = nullptr;
        FileReadingTest::SetUp();
    }
    void RunTest(chordalg::FileType file_type) {
        mock_file_->set_file_type(file_type);
        if (file_type == chordalg::FileType::DIMACS ||
            file_type == chordalg::FileType::ADJLIST) {
            file_reader_ = chordalg::NewFileReader< chordalg::GraphFR >(*mock_file_);
        } else if (file_type == chordalg::FileType::CHARACTERMATRIX ||
                   file_type == chordalg::FileType::NEXUSMRP) {
            file_reader_ = chordalg::NewFileReader< chordalg::CellIntGraphFR >(*mock_file_);
        }
    }
    void TearDown() {
        delete file_reader_;
        FileReadingTest::TearDown();
    }
 protected:
    chordalg::GraphFR* file_reader_;
};  // GetLinesTest

class IsomorphismTest : public FileReadingTest {
 public:
    void SetUp() {
        G_ = nullptr;
        FileReadingTest::SetUp();
    }
    void RunTest(chordalg::FileType file_type, chordalg::AdjacencyLists graph) {
        mock_file_->set_file_type(file_type);
        if (file_type == chordalg::FileType::DIMACS ||
            file_type == chordalg::FileType::ADJLIST) {
            G_ = new chordalg::Graph(
                    chordalg::NewFileReader< chordalg::GraphFR >(*mock_file_));
        } else if (file_type == chordalg::FileType::CHARACTERMATRIX ||
                   file_type == chordalg::FileType::NEXUSMRP) {
            G_ = new chordalg::CellIntersectionGraph(
                    chordalg::NewFileReader< chordalg::CellIntGraphFR >(*mock_file_));
        }
        chordalg::Graph H(new chordalg::AdjacencyLists(graph));
        EXPECT_EQ(G_->IsIsomorphic(H), true);
    }
    void TearDown() {
        delete G_;
        FileReadingTest::TearDown();
    }
 protected:
    chordalg::Graph* G_;
};  // IsomorphismTest

////////
// Tests

TEST_F(GetLinesTest, AdjListGetLines) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(4)
        .WillOnce(GraphFileInputLine("2", line_stream_))
        .WillOnce(GraphFileInputLine("1 2", line_stream_))
        .WillOnce(GraphFileInputLine("2 1", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::ADJLIST);
}

TEST_F(GetLinesTest, CharacterMatrixGetLines) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(5)
        .WillOnce(GraphFileInputLine("id", line_stream_))
        .WillOnce(GraphFileInputLine("2 2", line_stream_))
        .WillOnce(GraphFileInputLine("0 0", line_stream_))
        .WillOnce(GraphFileInputLine("0 1", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::CHARACTERMATRIX);
}

TEST_F(GetLinesTest, NexusMRPGetLines) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(10)
        .WillOnce(GraphFileInputLine("#NEXUS", line_stream_))
        .WillOnce(GraphFileInputLine("Begin Data", line_stream_))
        .WillOnce(GraphFileInputLine("Dimensions ntax=2 nchar=2", line_stream_))
        .WillOnce(GraphFileInputLine("Format datatype=standard symbols=\"01\"", line_stream_))
        .WillOnce(GraphFileInputLine("Matrix", line_stream_))
        .WillOnce(GraphFileInputLine("taxon1 01", line_stream_))
        .WillOnce(GraphFileInputLine("taxon2 11", line_stream_))
        .WillOnce(GraphFileInputLine(";", line_stream_))
        .WillOnce(GraphFileInputLine("End;", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::NEXUSMRP);
}

// The integration tests below are brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// how the file reader parses a graph file.

// Compares the adjacency list to the Dimacs format representation
TEST_F(IsomorphismTest, DimacsIsomorphism) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(22)
        .WillOnce(GraphFileInputLine("p edge 11 20", line_stream_))
        .WillOnce(GraphFileInputLine("e 1 2", line_stream_))
        .WillOnce(GraphFileInputLine("e 1 4", line_stream_))
        .WillOnce(GraphFileInputLine("e 1 7", line_stream_))
        .WillOnce(GraphFileInputLine("e 1 9", line_stream_))
        .WillOnce(GraphFileInputLine("e 2 3", line_stream_))
        .WillOnce(GraphFileInputLine("e 2 6", line_stream_))
        .WillOnce(GraphFileInputLine("e 2 8", line_stream_))
        .WillOnce(GraphFileInputLine("e 3 5", line_stream_))
        .WillOnce(GraphFileInputLine("e 3 7", line_stream_))
        .WillOnce(GraphFileInputLine("e 3 10", line_stream_))
        .WillOnce(GraphFileInputLine("e 4 5", line_stream_))
        .WillOnce(GraphFileInputLine("e 4 6", line_stream_))
        .WillOnce(GraphFileInputLine("e 4 10", line_stream_))
        .WillOnce(GraphFileInputLine("e 5 8", line_stream_))
        .WillOnce(GraphFileInputLine("e 5 9", line_stream_))
        .WillOnce(GraphFileInputLine("e 6 11", line_stream_))
        .WillOnce(GraphFileInputLine("e 7 11", line_stream_))
        .WillOnce(GraphFileInputLine("e 8 11", line_stream_))
        .WillOnce(GraphFileInputLine("e 9 11", line_stream_))
        .WillOnce(GraphFileInputLine("e 10 11", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::DIMACS, dimacs_test);
}

// Compares the adjacency list to the matrix representation
TEST_F(IsomorphismTest, CharacterMatrixIsomorphism) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(11)
        .WillOnce(GraphFileInputLine("id", line_stream_))
        .WillOnce(GraphFileInputLine("8 6", line_stream_))
        .WillOnce(GraphFileInputLine("? 0 ? 0 0 0", line_stream_))
        .WillOnce(GraphFileInputLine("1 1 1 0 0 1", line_stream_))
        .WillOnce(GraphFileInputLine("? ? 2 1 ? 2", line_stream_))
        .WillOnce(GraphFileInputLine("3 1 1 0 0 ?", line_stream_))
        .WillOnce(GraphFileInputLine("2 2 2 1 1 ?", line_stream_))
        .WillOnce(GraphFileInputLine("3 1 1 0 ? ?", line_stream_))
        .WillOnce(GraphFileInputLine("? 0 1 0 0 0", line_stream_))
        .WillOnce(GraphFileInputLine("0 0 0 ? 0 0", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::CHARACTERMATRIX, cig_test);
}

// Compares the adjacency list to the matrix representation
TEST_F(IsomorphismTest, NexusMRPIsomorphism) {
    EXPECT_CALL((*mock_file_), GetLine(_))
        .Times(11)
        .WillOnce(GraphFileInputLine("#NEXUS", line_stream_))
        .WillOnce(GraphFileInputLine("Begin Data", line_stream_))
        .WillOnce(GraphFileInputLine("Dimensions ntax=3 nchar=3", line_stream_))
        .WillOnce(GraphFileInputLine("Format datatype=standard symbols=\"01\" Missing=?", line_stream_))
        .WillOnce(GraphFileInputLine("Matrix", line_stream_))
        .WillOnce(GraphFileInputLine("taxon1 000", line_stream_))
        .WillOnce(GraphFileInputLine("taxon2 010", line_stream_))
        .WillOnce(GraphFileInputLine("root ?01", line_stream_))
        .WillOnce(GraphFileInputLine(";", line_stream_))
        .WillOnce(GraphFileInputLine("End;", line_stream_))
        .WillOnce(ReturnRef(eof_marker_));
    RunTest(chordalg::FileType::NEXUSMRP, nexus_test);
}

