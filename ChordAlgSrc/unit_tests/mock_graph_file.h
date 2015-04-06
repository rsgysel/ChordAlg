#include <iostream>
#include <string>

#include "gmock/gmock.h"

#include "ChordAlgSrc/graph_file.h"

// For specifying input files of MockGraphFile via GetLine.
// SetArgReferee<0>(str): sets the first param of fn to str
#define GraphFileInputLine(line, stream) \
    (DoAll(SetArgReferee<0>(line), ReturnRef(stream)))

class MockGraphFile : public chordalg::GraphFile {
 public:
    MockGraphFile() {}
    ~MockGraphFile() {}
    MOCK_METHOD1(GetLine, std::istream&(std::string& str));
};  // MockGraphFile
