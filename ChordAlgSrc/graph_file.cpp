#include "graph_file.h"

namespace chordalg {

GraphFile::GraphFile(std::string filename) {
    file_stream_.open(filename.c_str());
    if (!file_stream_) {
        std::cerr << "ChordAlg: Error opening file " << filename << ".";
        std::cerr << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}

GraphFile::~GraphFile() {
    file_stream_.close();
    return;
}

std::istream& GraphFile::GetLine(std::string& str) {
    return getline(file_stream_, str);
}

} // namespace chordalg
