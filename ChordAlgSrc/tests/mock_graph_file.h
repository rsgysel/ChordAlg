#ifndef INCLUDE_MOCK_GRAPH_FILE_H_
#define INCLUDE_MOCK_GRAPH_FILE_H_

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
    MockGraphFile(chordalg::FileType file_type) : chordalg::GraphFile(file_type) {}
    ~MockGraphFile() {}

    void set_file_type(chordalg::FileType file_type) {
        file_type_ = file_type;
    }

    MOCK_METHOD1(GetLine, std::istream&(std::string& str));
};  // MockGraphFile

#endif  // INCLUDE_MOCK_GRAPH_FILE_H_
