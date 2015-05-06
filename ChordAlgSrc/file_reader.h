/*
 *  file_reader.h - graph file parsers
 */

#ifndef CHORDALGSRC_FILE_READER_H_
#define CHORDALGSRC_FILE_READER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "ChordAlgSrc/graph_file.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef std::vector< std::list< std::string > > StringAdjLists;

class GraphFR {
 public:
    GraphFR() = delete;
    GraphFR(const GraphFR&) = delete;
    void operator=(const GraphFR&) = delete;
    virtual ~GraphFR();

    AdjacencyLists* TakeNeighborhoods();
    VertexNames* TakeNames();
    static GraphFR* New(std::string);
    static GraphFR* New(GraphFile*);

 protected:
    explicit GraphFR(GraphFile*);
    explicit GraphFR(std::string);

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
    std::vector< FiniteSet > subsets() const;
    std::vector< std::string > taxon_name() const;

 protected:
    explicit CharacterIntGraphFR(GraphFile*);

    void ReadFileOrDie();
    void ComputeGraphData();
    void TieSubsetsToVertices(const CharacterMatrix*);
    virtual void AddVertex(FiniteSet, const CharacterMatrix*, size_t) = 0;
    CharacterMatrix* ParseMatrix();
    CharacterMatrix* ParseNexusMRP();

    std::vector< FiniteSet > subsets_;
    std::vector< std::string > taxon_name_;
};  // CharacterIntGraphFR

class PartitionIntGraphFR : public CharacterIntGraphFR {
 public:
    PartitionIntGraphFR() = delete;
    PartitionIntGraphFR(const PartitionIntGraphFR&) = delete;
    void operator=(const PartitionIntGraphFR&) = delete;

    virtual ~PartitionIntGraphFR();

    std::vector< Color > vertex_color() const;
    static PartitionIntGraphFR* New(std::string);
    static PartitionIntGraphFR* New(GraphFile*);

 protected:
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
    std::vector< Multicolor > vertex_colors() const;

    static CellIntGraphFR* New(std::string);
    static CellIntGraphFR* New(GraphFile*);

 protected:
    explicit CellIntGraphFR(GraphFile*);

    void AddVertex(FiniteSet, const CharacterMatrix*, size_t);

    LexTrie* subset_family_;
    std::vector< Multicolor > vertex_colors_;

    std::map< const LexTrieNode*, Vertex > cell_id_;
};  // CellIntGraphFR

}  // namespace chordalg

#endif  // CHORDALGSRC_FILE_READER_H_
