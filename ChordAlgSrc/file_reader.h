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

#include "chordalg_string.h"
#include "graph_file.h"
#include "lex_trie.h"
#include "vertices.h"

namespace chordalg {

typedef std::vector< std::list< std::string > > StringAdjLists;

// Abstract class used to read files for graphs.
// To design a file reader class ClassFR, include the following:
//      1) private constructor ClassFR(std::string) that initializes FileReader
//          with input string
//      2) private ReadFileOrDie() method. you must close file_stream_ here
//      3) friend access to factory method template< class FR > friend FR*
//          NewFileReader(std::string);
class GraphFR {
 public:
    GraphFR() = delete;
    GraphFR(const GraphFR&) = delete;
    void operator=(const GraphFR&) = delete;
    virtual ~GraphFR();

    AdjacencyLists* TakeNeighborhoods();
    VertexNames* TakeNames();
 protected:
    // construction and initialization deferred to NewFileReader
    template< class FR > friend FR* NewFileReader(GraphFile& file);
    explicit GraphFR(GraphFile* file);
    explicit GraphFR(std::string filename);

    virtual void ReadFileOrDie();
    void ComputeGraphData(const StringAdjLists*);
    void ComputeNames(size_t);

    void ParseDimacs(StringAdjLists*);
    void ParseAdjList(StringAdjLists*);

    GraphFile* file_;
    AdjacencyLists* neighborhoods_;
    VertexNames* names_;

    std::map< std::string, Vertex > vertex_id_;
};  // GraphFR

//////////////////////////////////
// FR must be derived from GraphFR

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
    GraphFR* type_check = fr_object;
    (void) type_check;
    return fr_object;
}

class CharacterMatrix {
 public:
    CharacterMatrix() = delete;
    CharacterMatrix(const CharacterMatrix&) = delete;
    void operator=(const CharacterMatrix&) = delete;

    CharacterMatrix(size_t, size_t);
    CharacterMatrix(size_t, size_t, size_t);

    std::vector< size_t >& operator[](size_t);
    const std::vector< size_t >& operator[](size_t i) const;

    void set_max_states(size_t);
    size_t cols() const;
    size_t rows() const;
    size_t max_states() const;
    static size_t kMissingData();
    std::string str() const;
 private:
    std::vector< std::vector< size_t > > M_;
    size_t cols_;
    size_t rows_;
    size_t max_states_;
};  // CharacterMatrix

class CharacterIntGraphFR : public GraphFR {
 public:
    CharacterIntGraphFR() = delete;
    CharacterIntGraphFR(const CharacterIntGraphFR&) = delete;
    void operator=(const CharacterIntGraphFR&) = delete;

    virtual ~CharacterIntGraphFR();
    std::vector< FiniteSet > subsets() const {
        return subsets_;
    }
    std::vector< std::string > taxon_name() const {
        return taxon_name_;
    }
 protected:
    template< class FR > friend FR* NewFileReader(GraphFile&);

    explicit CharacterIntGraphFR(GraphFile*);
    void ReadFileOrDie();


    void ComputeGraphData();
    void TieSubsetsToVertices(const CharacterMatrix*);
    virtual void AddVertex(FiniteSet, const CharacterMatrix*, size_t) = 0;

    CharacterMatrix* ParseNexusMRP();
    CharacterMatrix* ParseMatrix();

    std::string ParseNexusParameter(std::string, std::string) const;

    std::vector< FiniteSet > subsets_;
    std::vector< std::string > taxon_name_;

};  // CharacterIntGraphFR

class PartitionIntGraphFR : public CharacterIntGraphFR {
 public:
    PartitionIntGraphFR() = delete;
    PartitionIntGraphFR(const PartitionIntGraphFR&) = delete;
    void operator=(const PartitionIntGraphFR&) = delete;

    virtual ~PartitionIntGraphFR();

    std::vector< Color> vertex_color() const {
        return vertex_color_;
    }
 protected:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit PartitionIntGraphFR(GraphFile*);

    void AddVertex(FiniteSet, const CharacterMatrix*, size_t);

    std::vector< Color > vertex_color_;
};  // PartitionIntGraphFR

class CellIntGraphFR : public CharacterIntGraphFR {
 public:
    CellIntGraphFR() = delete;
    CellIntGraphFR(const CellIntGraphFR&) = delete;
    void operator=(const CellIntGraphFR&) = delete;

    virtual ~CellIntGraphFR();

    LexTrie* TakeSubsetFamily();
    std::vector< Multicolor > vertex_colors() const {
        return vertex_colors_;
    }
 protected:
    template< class FR > friend FR* NewFileReader(GraphFile&);
    explicit CellIntGraphFR(GraphFile*);

    void AddVertex(FiniteSet, const CharacterMatrix*, size_t);

    LexTrie* subset_family_;
    std::vector< Multicolor > vertex_colors_;

    std::map< const LexTrieNode*, Vertex > cell_id_;
};  // CellIntGraphFR

}  // namespace chordalg

#endif  // INCLUDE_FILE_READER_H_
