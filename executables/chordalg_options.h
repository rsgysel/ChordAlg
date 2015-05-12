#include <getopt.h>

#include <iostream>
#include <string>

enum class FileMode {
    GRAPH,
    CHARACTERMATRIX
};  // FileMode

void ChordAlgOptions(int argc,
                     char* argv[],
                     std::string* graph_filename,
                     std::string* eo_filename = nullptr,
                     std::string* triangulation_filename = nullptr,
                     FileMode mode = FileMode::GRAPH);
