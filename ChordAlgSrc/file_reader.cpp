#include "file_reader.h"

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

//////////////////
// c'tors & d'tors

GraphFR::GraphFR(GraphFile* file) :
    file_(file),
    neighborhoods_(nullptr),
    names_(nullptr) {
    return;
}

GraphFR::GraphFR(std::string filename) :
    file_(new GraphFile(filename)),
    neighborhoods_(nullptr),
    names_(nullptr) {
    return;
}

GraphFR::~GraphFR() {
    delete neighborhoods_;
    delete names_;
    return;
}

CharacterMatrix::CharacterMatrix(size_t rows, size_t cols) :
    M_(rows, std::vector< size_t >(cols)),
    cols_(cols),
    rows_(rows),
    max_states_(0) {
    return;
}

CharacterMatrix::CharacterMatrix(size_t rows, size_t cols, size_t max_states) :
    M_(rows, std::vector< size_t >(cols)),
    cols_(cols),
    rows_(rows),
    max_states_(max_states) {
    return;
}

CharacterIntGraphFR::CharacterIntGraphFR(GraphFile* file) :
    GraphFR(file) {
    return;
}

CharacterIntGraphFR::~CharacterIntGraphFR() {
    return;
}

PartitionIntGraphFR::PartitionIntGraphFR(GraphFile* file) :
    CharacterIntGraphFR(file) {
    return;
}

PartitionIntGraphFR::~PartitionIntGraphFR() {
    return;
}

CellIntGraphFR::CellIntGraphFR(GraphFile* file) :
    CharacterIntGraphFR(file),
    subset_family_(nullptr) {
    return;
}

CellIntGraphFR::~CellIntGraphFR() {
    delete subset_family_;
    return;
}

//////////////////
// CharacterMatrix

std::vector< size_t >& CharacterMatrix::operator[](size_t i) {
    return M_[i];
}

const std::vector< size_t >& CharacterMatrix::operator[](size_t i) const {
    return M_[i];
}

size_t CharacterMatrix::cols() const {
    return cols_;
}

size_t CharacterMatrix::rows() const {
    return rows_;
}

void CharacterMatrix::set_max_states(size_t max_states) {
    max_states_ = max_states;
}

size_t CharacterMatrix::max_states() const {
    return max_states_;
}

size_t CharacterMatrix::kMissingData() {
    return SIZE_MAX;
}

std::string CharacterMatrix::str() const {
    std::string str = std::to_string(rows_) + " "
                    + std::to_string(cols_) + "\n";
    for (auto row : M_) {
        for (auto val : row) {
            if (val == kMissingData()) {
                str += "? ";
            } else {
                str += std::to_string(val) + " ";
            }
        }
        str.pop_back();
        str += "\n";
    }
    return str;
}

//////////////////
// data for graphs

AdjacencyLists* GraphFR::TakeNeighborhoods() {
    AdjacencyLists* temp = nullptr;
    std::swap(temp, neighborhoods_);
    return temp;
}

VertexNames* GraphFR::TakeNames() {
    VertexNames* temp = nullptr;
    std::swap(temp, names_);
    return temp;
}

LexTrie* CellIntGraphFR::TakeSubsetFamily() {
    LexTrie* temp = nullptr;
    std::swap(temp, subset_family_);
    return temp;
}

//////////////////
// file processing

void GraphFR::ReadFileOrDie() {
    FileType file_type = file_->file_type();
    StringAdjLists* adjacency_lists = new StringAdjLists();
    if (file_type == FileType::ADJLIST) {
        ParseAdjList(adjacency_lists);
    } else if (file_type == FileType::DIMACS) {
        ParseDimacs(adjacency_lists);
    } else {
        std::cerr << "Unrecognized file type\n";
        exit(EXIT_FAILURE);
    }
    ComputeGraphData(adjacency_lists);
    delete adjacency_lists;
    return;
}

void CharacterIntGraphFR::ReadFileOrDie() {
    FileType file_type = file_->file_type();
    CharacterMatrix* M = nullptr;
    if (file_type == FileType::NEXUSMRP) {
        M = ParseNexusMRP();
    } else if (file_type == FileType::CHARACTERMATRIX) {
        M = ParseMatrix();
    } else {
        std::cerr << "Unrecognized file type\n";
        exit(EXIT_FAILURE);
    }
    TieSubsetsToVertices(M);
    ComputeGraphData();
    delete M;
    return;
}

void GraphFR::ComputeGraphData(
    const StringAdjLists* adjacency_lists) {
    // String to Vertices
    neighborhoods_ = new AdjacencyLists;
    size_t order = adjacency_lists->size();
    ComputeNames(order);
    neighborhoods_->resize(order);
    for (size_t i = 0; i < order; ++i) {
        Vertices* nbhd = &(*neighborhoods_)[i];
        for (std::string neighbor : (*adjacency_lists)[i]) {
            Vertex v = vertex_id_[neighbor];
            nbhd->add(v);
        }
        std::sort(nbhd->begin(), nbhd->end());
    }
}

void GraphFR::ComputeNames(size_t order) {
    names_ = new VertexNames(order);
    for (auto p : vertex_id_) {
        std::string name = p.first;
        Vertex v = p.second;
        (*names_)[v] = name;
    }
    return;
}   

void CharacterIntGraphFR::ComputeGraphData() {
    size_t order = subsets_.size();
    ComputeNames(order);
    neighborhoods_ = new AdjacencyLists(order);
    Vertex v = 0;
    size_t rows = taxon_name_.size();
    std::vector< VertexVector > taxon_clique(rows);
    for (FiniteSet& C : subsets_) {
        for (size_t t : C) {
            taxon_clique[t].push_back(v);
        }
        v++;
    }
    std::map< VertexPair, bool > edges;
    for (VertexVector V : taxon_clique) {
        for (Vertex v : V) {
            for (Vertex u : V) {
                if (u < v) {
                    edges[VertexPair(u, v)] = true;
                }
            }
        }
    }
    for (std::pair< VertexPair, bool > p : edges) {
        VertexPair e = p.first;
        Vertex u = e.first, v = e.second;
        (*neighborhoods_)[u].add(v);
        (*neighborhoods_)[v].add(u);
    }
    for (Vertices& V : *neighborhoods_) {
        std::sort(V.begin(), V.end());
    }
    return;
}

void CharacterIntGraphFR::TieSubsetsToVertices(const CharacterMatrix* M) { 
    size_t rows = M->rows();
    size_t cols = M->cols();
    for (size_t j = 0; j < cols; ++j) {
        std::vector< FiniteSet > cells(M->max_states(), FiniteSet());
        for (size_t i = 0; i < rows; ++i) {
            size_t state = (*M)[i][j];
            if (state != M->kMissingData()) {
                cells[state].push_back(i);
            }
        }
        for (FiniteSet& C : cells) {
            AddVertex(C, M, j);
        }
    }
    return;
}

void PartitionIntGraphFR::AddVertex(
    FiniteSet C, 
    const CharacterMatrix* M,
    size_t col) {
    if (!C.empty()) {
        size_t v = subsets_.size();
        size_t state = (*M)[C[0]][col];
        std::string name = std::to_string(col) + "#" + std::to_string(state);
        vertex_id_[name] = v;
        subsets_.push_back(FiniteSet(C));
        vertex_color_.push_back(col);
    }
    return;
}

void CellIntGraphFR::AddVertex(
    FiniteSet C,
    const CharacterMatrix* M,
    size_t col) {
    if (!C.empty()) {
        if(!subset_family_) {
            subset_family_ = new LexTrie(M->rows());
        }
        bool new_cell;
        std::sort(C.begin(), C.end());
        const LexTrieNode* node =
                subset_family_->Insert< FiniteSet >(C, &new_cell);
        if (new_cell) {
            size_t cell = subsets_.size();
            cell_id_[node] = cell;
            size_t state = (*M)[C[0]][col];
            std::string name = std::to_string(col) + "#" + std::to_string(state);
            vertex_id_[name] = cell;
            subsets_.push_back(FiniteSet(C));
            vertex_colors_.push_back(Multicolor());
        }
        Vertex v = cell_id_[node];
        vertex_colors_[v].push_back(col);
    }
    return;
}

////////////////////////
// File-specific Parsers

// Dimacs Format:
// c This is a comment line
// p FORMAT NODES EDGES // we ignore FORMAT, NODES = number of nodes, etc.
// e U V // edge descriptors: (U,V) = e is an edge of G. not to be duplicated
// with e V U. U and V are integers in 1, 2, ..., n
void GraphFR::ParseDimacs(
    StringAdjLists* adjacency_lists) {
    size_t order;
    std::string line;
    while (file_->GetLine(line)) {
        StringTokens line_tokens = Split(line, " \t");
        switch (line_tokens[0][0]) {
            case 'c' : {
                break;
            }
            case 'e' : {
                if (vertex_id_.find(line_tokens[1]) == vertex_id_.end()) {
                    size_t new_vertex_id = vertex_id_.size();
                    vertex_id_[line_tokens[1]] = new_vertex_id;
                }
                if (vertex_id_.find(line_tokens[2]) == vertex_id_.end()) {
                    size_t new_vertex_id = vertex_id_.size();
                    vertex_id_[line_tokens[2]] = new_vertex_id;
                }
                Vertex v1 = vertex_id_[line_tokens[1]], v2 = vertex_id_[line_tokens[2]];
                (*adjacency_lists)[v1].push_back(line_tokens[2]);
                (*adjacency_lists)[v2].push_back(line_tokens[1]);
                break;
            }
            case 'p' : {
                order = std::stoi(line_tokens[2]);
                adjacency_lists->resize(order);
                break;
            }
        }
    }
    return;
}

// Adjacency List Format:
// Line 1: non-negative integer denoting the number of vertices
// Line i>1: i-1, representing vertex i-1, followed by its neighbors as sorted
// integers delimited by whitespace
void GraphFR::ParseAdjList(
    StringAdjLists* adjacency_lists) {
    // First line should be number of vertices
    std::string line;
    file_->GetLine(line);
    size_t order = std::stoi(line);
    adjacency_lists->resize(order);
    // Next lines should be adjacency lists
    size_t row = 0;
    while (file_->GetLine(line)) {
        StringTokens line_tokens = Split(line, " \t");
        std::string vertex = line_tokens[0];
        size_t new_vertex_id = vertex_id_.size();
        vertex_id_[vertex] = new_vertex_id;
        for (size_t i = 1; i < line_tokens.size(); ++i) {
            (*adjacency_lists)[row].push_back(line_tokens[i]);
        }
        ++row;
    }
    return;    
}

CharacterMatrix* CharacterIntGraphFR::ParseMatrix() {
    // First line is the name of the experiment, and is arbitrary
    std::string line;
    file_->GetLine(line);
    // Second line is the number of rows and columns
    file_->GetLine(line);
    StringTokens line_tokens = Split(line, " \t");
    size_t rows = std::stoi(line_tokens[0]), cols = std::stoi(line_tokens[1]);
    CharacterMatrix* M = new CharacterMatrix(rows, cols);
    size_t i = 0;
    while (file_->GetLine(line)) {
        line_tokens = Split(line, " \t");
        for (size_t j = 0; j < cols; ++j) {
            std::string state = line_tokens[j];
            if (IsNum(state)) {
                size_t s = std::stoi(state);
                (*M)[i][j] = s;
                if (s + 1 > M->max_states()) {
                    M->set_max_states(s + 1);
                }
            } else {
                (*M)[i][j] = M->kMissingData();
            }
        }
        ++i;
    }
    // default taxon names
    taxon_name_.resize(rows);
    for (size_t i = 0; i < rows; ++i) {
        taxon_name_[i] = std::to_string(i);
    }
    return M;
}

CharacterMatrix* CharacterIntGraphFR::ParseNexusMRP() {
    // Line 1: #NEXUS
    std::string line;
    StringTokens line_tokens;
    file_->GetLine(line);
    // Line 2: Begin Data;
    do {
        file_->GetLine(line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    // Line 3: Dimensions ntax = 3, nchar = 3;
    do {
        file_->GetLine(line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    line_tokens = Split(line, " \t=;");
    size_t rows = std::stoi(line_tokens[2]);
    size_t cols = std::stoi(line_tokens[4]);
    // Line 4: Format datatype=standard symbols="01" Missing=?;
    do {
        file_->GetLine(line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    // Line 5: Matrix
    do {
        file_->GetLine(line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    // Matrix lines: taxon_name 0101?
    CharacterMatrix* M = new CharacterMatrix(rows, cols, 2);
    taxon_name_.resize(rows);
    size_t i = 0;
    while (file_->GetLine(line) &&
           line.compare(";") != 0) {
        line_tokens = Split(line, " \t");
        taxon_name_[i] = line_tokens[0];
        for (size_t j = 0; j < line_tokens[1].length(); ++j) {
            switch (line_tokens[1][j]) {
                case '0' : {
                    (*M)[i][j] = 0;
                    break;
                }
                case '1' : {
                    (*M)[i][j] = 1;
                    break;
                }
                default : {
                    (*M)[i][j] = M->kMissingData();
                    break;
                }
            }            
        }
        ++i;
    }
    // Last line: end;
    file_->GetLine(line);
    // Whitespace
    while (file_->GetLine(line)) {}
    return M;
}

}  // namespace chordalg
