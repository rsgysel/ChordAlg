#ifndef INCLUDE_FILE_GRAPH_FILE_H_
#define INCLUDE_FILE_GRAPH_FILE_H_

#include <fstream>
#include <iostream>
#include <string>

namespace chordalg {

class GraphFile {
 public:
    GraphFile(std::string filename);
    ~GraphFile();
    
    std::istream& GetLine(std::string& str);

 protected:
    GraphFile() {} // for testing
 private:
    std::ifstream file_stream_;
};

} // namespace chordalg

#endif // INCLUDE_FILE_READER_H_
