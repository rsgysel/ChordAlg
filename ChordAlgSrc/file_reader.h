/*
 *  file_reader.h - graph file parsers
 */

#ifndef INCLUDE_FILE_READER_H_
#define INCLUDE_FILE_READER_H_

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "graph_file.h"
#include "lex_trie.h"
#include "vertices.h"

namespace chordalg {

// Abstract class used to read files for graphs.
// To design a file reader class ClassFR, include the following:
//      1) private constructor ClassFR(std::string) that initializes FileReader
//          with input string
//      2) private ReadFileOrDie() method. you must close file_stream_ here
//      3) friend access to factory method template< class FR > friend FR*
//          NewFileReader(std::string);
class FileReader {
 public:
    FileReader() = delete;
    virtual ~FileReader();

    AdjacencyLists* TakeNeighborhoods();
    VertexNames* TakeNames();
 protected:
    explicit FileReader(GraphFile* file);
    explicit FileReader(std::string filename);
    inline void AssertFormatOrDie(bool, std::string) const;

    GraphFile* file_;
    AdjacencyLists* neighborhoods_;
    VertexNames* names_;
 private:
    // construction and initialization deferred to NewFileReader
    template< class FR > friend FR* NewFileReader(GraphFile& file);
    virtual void ReadFileOrDie() = 0;
};  // FileReader

// FR must be derived from FileReader
//
template< class FR >
FR* NewFileReader(std::string file_name) {
    GraphFile file(file_name);
    return NewFileReader< FR >(file);
}

template< class FR >
FR* NewFileReader(GraphFile& file) {
    // ctor
    FR* fr_object = new FR(&file);
    fr_object->ReadFileOrDie();
    // type check FR. voided to prevent compiler warning
    FileReader* type_check = fr_object;
    (void) type_check;
    return fr_object;
}

class DimacsGraphFR : public FileReader {
 public:
    ~DimacsGraphFR() {}
 private:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit DimacsGraphFR(GraphFile* file) : FileReader(file) {}
    void ReadFileOrDie();
};  // DimacsGraphFR

// FileReader for sorted adjacency list (.sal) files having format:
//      Line 1: non-negative integer denoting the number of vertices
//      Line i>1: i-1, representing vertex i-1, followed by its neighbors as
//          sorted integers delimited by whitespace
class SortedAdjacencyListFR : public FileReader {
 public:
    ~SortedAdjacencyListFR() {}

 private:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit SortedAdjacencyListFR(GraphFile* file) : FileReader(file) {}
    void ReadFileOrDie();
};  // SortedAdjacencyListFR

enum class FileType {
    DIMACS,
    MATRIX,
    NEXUSMRP,       // Nexus Matrix rep w/ Parsimony format
    SORTEDADJLIST
};  // FileType

typedef std::vector< std::vector< size_t > > CharacterMatrix;

class CharacterIntGraphFR : public FileReader {
 public:
    CharacterIntGraphFR() = delete;
    CharacterIntGraphFR(const CharacterIntGraphFR&) = delete;
    void operator=(const CharacterIntGraphFR&) = delete;

    virtual ~CharacterIntGraphFR();
    std::vector< std::string > taxon_name() const {
        return taxon_name_;
    }
 protected:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit CharacterIntGraphFR(GraphFile* file);
    void ReadFileOrDie();
    FileType GetFileType();
    CharacterMatrix* ParseNexusMRP(size_t*);
    CharacterMatrix* ParseMatrix(size_t*);
    virtual void ComputeGraphData(const CharacterMatrix*, size_t) = 0;

    int kMissingData() {
        return -1;
    }

    std::string ParseNexusParameter(std::string, std::string) const;

    std::vector< std::string > taxon_name_;
};  // CharacterIntGraphFR

class CellIntGraphFR : public CharacterIntGraphFR {
 public:
    CellIntGraphFR() = delete;
    CellIntGraphFR(const CellIntGraphFR&) = delete;
    void operator=(const CellIntGraphFR&) = delete;

    virtual ~CellIntGraphFR();

    LexTrie* TakeSubsetFamily();
    std::vector< FiniteSet > subsets() const {
        return subsets_;
    }
    std::vector< Multicolor > vertex_colors() const {
        return vertex_colors_;
    }
 protected:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit CellIntGraphFR(GraphFile* file);
    void ComputeGraphData(const CharacterMatrix*, size_t);

    LexTrie* subset_family_;
    std::vector< FiniteSet > subsets_;
    std::vector< Multicolor > vertex_colors_;
};  // CellIntGraphFR

}  // namespace chordalg

#endif  // INCLUDE_FILE_READER_H_
