#include <fstream>
#include <string>
#include <typeinfo>

#include "gtest/gtest.h"

#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/graph_file.h"
#include "mock_graph_file.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

/////////////
// Frameworks

template< class FileReaderType >
class FileReadingTest : public ::testing::Test {
  public:
    void SetUp() {
        eof_marker_.peek(); eof_marker_.peek(); // Twice for end-of-file bit
    }
    void RunTest() {
        file_reader_ = chordalg::NewFileReader< FileReaderType >(mock_file_);
    }
    void TearDown() {
        delete file_reader_;
    }
    bool TypeIs(const std::type_info& RHS_Type) const {
        return typeid(FileReaderType) == RHS_Type;
    }
  protected:
    MockGraphFile mock_file_;
    FileReaderType* file_reader_;
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
            .WillOnce(GraphFileInputLine("2", this->line_stream_))
            .WillOnce(GraphFileInputLine("1 2", this->line_stream_))
            .WillOnce(GraphFileInputLine("2 1", this->line_stream_))
            .WillOnce(ReturnRef(this->eof_marker_));
    } else if(this->TypeIs(typeid(chordalg::MatrixCellIntGraphFR))) {
        EXPECT_CALL(this->mock_file_, GetLine(_))
            .Times(5)
            .WillOnce(GraphFileInputLine("id", this->line_stream_))
            .WillOnce(GraphFileInputLine("2 2", this->line_stream_))
            .WillOnce(GraphFileInputLine("0 0", this->line_stream_))
            .WillOnce(GraphFileInputLine("0 1", this->line_stream_))
            .WillOnce(ReturnRef(this->eof_marker_));
    } else if(this->TypeIs(typeid(chordalg::NexusMRPFR))) {
        EXPECT_CALL(this->mock_file_, GetLine(_))
            .Times(10)
            .WillOnce(GraphFileInputLine("#NEXUS", this->line_stream_))
            .WillOnce(GraphFileInputLine("Begin Data", this->line_stream_))
            .WillOnce(GraphFileInputLine("Dimensions ntax=2 nchar=2", this->line_stream_))
            .WillOnce(GraphFileInputLine("Format datatype=standard symbols=\"01\"", this->line_stream_))
            .WillOnce(GraphFileInputLine("Matrix", this->line_stream_))
            .WillOnce(GraphFileInputLine("taxon1 01", this->line_stream_))
            .WillOnce(GraphFileInputLine("taxon2 11", this->line_stream_))
            .WillOnce(GraphFileInputLine(";", this->line_stream_))
            .WillOnce(GraphFileInputLine("End;", this->line_stream_))
            .WillOnce(ReturnRef(this->eof_marker_));
    }
    this->RunTest();
}
