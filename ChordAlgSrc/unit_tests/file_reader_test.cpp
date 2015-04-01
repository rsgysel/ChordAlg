#include <fstream>
#include <sstream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/file_reader.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

class MockGraphFile : public chordalg::GraphFile {
 public:
    MockGraphFile() {}
    ~MockGraphFile() {}
    MOCK_METHOD1(GetLine, std::istream&(std::string& str));
};  // MockGraphFile

TEST(FileReaderTest, GetLines) {
    MockGraphFile file;
    std::istringstream l1("2"), l2("1 2"), l3("2 1");
    std::ifstream eof_marker;
    eof_marker.peek();
    EXPECT_CALL(file, GetLine(_))
        .Times(4)
        .WillOnce(ReturnRef(l1))
        .WillOnce(ReturnRef(l2))
        .WillOnce(ReturnRef(l3))
        .WillOnce(ReturnRef(eof_marker));
    auto FR = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file);
    delete FR; 
}


