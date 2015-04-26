#ifndef INCLUDE_FILE_GRAPH_FILE_H_
#define INCLUDE_FILE_GRAPH_FILE_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "chordalg_string.h"

namespace chordalg {

enum class FileType {
    ADJLIST,
    CHARACTERMATRIX,
    DIMACS,
    NEXUSMRP,       // Nexus Matrix rep w/ Parsimony format
    UNKNOWN
};  // FileType

// Wrapper to facilitate testing
class GraphFile {
 public:
    GraphFile(const GraphFile&) = delete;
    GraphFile(std::string filename);
    virtual ~GraphFile();

    virtual std::istream& GetLine(std::string& str);
    FileType file_type() const {
        return file_type_;
    }
 protected:
    // Constructors for Mocking
    GraphFile();
    GraphFile(FileType);

    std::ifstream file_stream_;
    FileType file_type_;

    static void AssertOrDie(bool, std::string);
    static bool IsTwoStateTaxon(std::string, char);

    void AssignFileType(FileType);
    void GetFileType(std::string);
    bool IsAdjacencyListFile(std::string) const;
    bool IsDimacsFile(std::string) const;
    bool IsMatrixFile(std::string) const;
    bool IsNexusMRPFile(std::string) const;
};

} // namespace chordalg

#endif // INCLUDE_FILE_READER_H_
