#ifndef CHORDALGSRC_GRAPH_FILE_H_
#define CHORDALGSRC_GRAPH_FILE_H_

#include <fstream>
#include <iostream>
#include <string>

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
    virtual ~GraphFile();

    static GraphFile* New(std::string);

    virtual std::istream& GetLine(std::string&);
    FileType file_type() const;

 protected:
    GraphFile();
    explicit GraphFile(FileType);

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

}  // namespace chordalg

#endif  // CHORDALGSRC_GRAPH_FILE_H_
