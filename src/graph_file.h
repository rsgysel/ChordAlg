#ifndef INCLUDE_FILE_GRAPH_FILE_H_
#define INCLUDE_FILE_GRAPH_FILE_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

namespace chordalg {

// Wrapper to facilitate testing
class GraphFile {
 public:
    GraphFile() = delete;
    GraphFile(std::string filename);
    ~GraphFile();
    
    std::istream& GetLine(std::string& str);

 private:
    std::ifstream file_stream_;
};

} // namespace chordalg

#endif // INCLUDE_FILE_READER_H_
