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

TEST(FileReaderTest, GetLines) {
    MockGraphFile file;
    std::ifstream line_stream, eof_marker;
    eof_marker.peek(); eof_marker.peek(); // Twice for end-of-file bit
    EXPECT_CALL(file, GetLine(_))
        .Times(4)
        .WillOnce(DoAll(SetArgReferee<0>("2"), ReturnRef(line_stream)))
        .WillOnce(DoAll(SetArgReferee<0>("1 2"), ReturnRef(line_stream)))
        .WillOnce(DoAll(SetArgReferee<0>("2 1"), ReturnRef(line_stream)))
        .WillOnce(ReturnRef(eof_marker));
    auto FR = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file);
    delete FR; 
}


