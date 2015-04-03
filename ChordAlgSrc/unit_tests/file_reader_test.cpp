#include <fstream>
#include <sstream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/file_reader.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

class MockGraphFile : public chordalg::GraphFile {
 public:
    MockGraphFile() {}
    ~MockGraphFile() {}
    MOCK_METHOD1(GetLine, std::istream&(std::string& str));
};  // MockGraphFile

template< class FR >
class FileReadingTest : public ::testing::Test {
  public:
    void SetUp() {
        eof_marker_.peek(); eof_marker_.peek(); // Twice for end-of-file bit
    }
    void RunTest() {
        file_reader_ = chordalg::NewFileReader< FR >(mock_file_);
    }
    void TearDown() {
        delete file_reader_;
    }
  protected:
    MockGraphFile mock_file_;
    FR* file_reader_;
    std::ifstream line_stream_, eof_marker_;
};  // FileReadingTest

typedef ::testing::Types< chordalg::SortedAdjacencyListFR >
//                          chordalg::MatrixCellIntGraphFR,
//                          chordalg::NexusMRPFR > 
                          FileReaderTypes;

TYPED_TEST_CASE(FileReadingTest, FileReaderTypes);
TYPED_TEST(FileReadingTest, GetLines) {
    EXPECT_CALL(this->mock_file_, GetLine(_))
        .Times(4)
        .WillOnce(DoAll(SetArgReferee<0>("2"), ReturnRef(this->line_stream_)))
        .WillOnce(DoAll(SetArgReferee<0>("1 2"), ReturnRef(this->line_stream_)))
        .WillOnce(DoAll(SetArgReferee<0>("2 1"), ReturnRef(this->line_stream_)))
        .WillOnce(ReturnRef(this->eof_marker_));
    this->RunTest();
}

