#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/file_reader.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

////////////////////
// Mock Dependencies

class MockGraphFile : public chordalg::GraphFile {
 public:
    MockGraphFile() {}
    ~MockGraphFile() {}
    MOCK_METHOD1(GetLine, std::istream&(std::string& str));
};  // MockGraphFile

// For specifying input files of MockGraphFile via GetLine.
// SetArgReferee<0>(str): sets the first param of fn to str
#define GraphFileInputLine(str) \
    (DoAll(SetArgReferee<0>(str), ReturnRef(this->line_stream_)))

/////////////
// Frameworks

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
    bool TypeIs(const std::type_info& RHS_FR_type) const {
        return typeid(FR) == RHS_FR_type;
    }
  protected:
    MockGraphFile mock_file_;
    FR* file_reader_;
    std::ifstream line_stream_, eof_marker_;
};  // FileReadingTest

///////////////////////
// Declared Typed Tests

typedef ::testing::Types< chordalg::SortedAdjacencyListFR,
                          chordalg::MatrixCellIntGraphFR,
                          chordalg::NexusMRPFR > 
                          FileReaderTypes;

TYPED_TEST_CASE(FileReadingTest, FileReaderTypes);

//////////////
// Typed Tests

TYPED_TEST(FileReadingTest, GetLines) {
    if(this->TypeIs(typeid(chordalg::SortedAdjacencyListFR))) {
        EXPECT_CALL(this->mock_file_, GetLine(_))
            .Times(4)
            .WillOnce(GraphFileInputLine("2"))
            .WillOnce(GraphFileInputLine("1 2"))
            .WillOnce(GraphFileInputLine("2 1"))
            .WillOnce(ReturnRef(this->eof_marker_));
    } else if(this->TypeIs(typeid(chordalg::MatrixCellIntGraphFR))) {
        EXPECT_CALL(this->mock_file_, GetLine(_))
            .Times(5)
            .WillOnce(GraphFileInputLine("id"))
            .WillOnce(GraphFileInputLine("2 2"))
            .WillOnce(GraphFileInputLine("0 0"))
            .WillOnce(GraphFileInputLine("0 1"))
            .WillOnce(ReturnRef(this->eof_marker_));
    } else if(this->TypeIs(typeid(chordalg::NexusMRPFR))) {
        EXPECT_CALL(this->mock_file_, GetLine(_))
            .Times(10)
            .WillOnce(GraphFileInputLine("#NEXUS"))
            .WillOnce(GraphFileInputLine("Begin Data"))
            .WillOnce(GraphFileInputLine("Dimensions ntax=2 nchar=2"))
            .WillOnce(GraphFileInputLine("Format datatype=standard symbols=\"01\""))
            .WillOnce(GraphFileInputLine("Matrix"))
            .WillOnce(GraphFileInputLine("taxon1 01"))
            .WillOnce(GraphFileInputLine("taxon2 11"))
            .WillOnce(GraphFileInputLine(";"))
            .WillOnce(GraphFileInputLine("End;"))
            .WillOnce(ReturnRef(this->eof_marker_));
    }
    this->RunTest();
}

