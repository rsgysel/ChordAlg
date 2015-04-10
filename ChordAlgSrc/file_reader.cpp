#include "file_reader.h"

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

// Format error messages
// DimacsFileReader
const char* Dimacs_problem_line_count =
    "Format Error: Only one problem line allowed";
const char* Dimacs_problem_line_first =
    "Format Error: a problem line must occur before any edge line";
const char* Dimacs_duplicate_edge =
    "Format Error: duplicated edge(s) detected";
const char* Dimacs_unknown_character =
    "Format Error: line(s) detected that start with unrecognized format";
// FileReader
const char* File_first_line_format =
    "Format Error: First line must be a non-negative integer denoting the "
    "number of vertices";
const char* File_first_int_format =
    "Format Error: Line i must start with i-2 for i > 1";
const char* File_too_many_lines =
    "Format Error: Too many lines; first line specifies number of lines "
    "(vertices)";
// MatrixCellIntGraphReder
const char* Matrix_first_line_format =
    "Format Error: First line must be two non-negative integer denoting the "
    "number of rows and columns";
const char* Matrix_too_many_cols = "Format Error: Too many columns";
const char* Matrix_too_many_rows = "Format Error: Too many rows";
// NexusMRPFR
const char* Nexus_too_many_cols = "Format Error: Too many columns";
const char* Nexus_too_many_rows = "Format Error: Too many rows";
const char* Nexus_unknown_symbol = "Format Error: unknown symbol";


////////////// ctor & dtors
//
FileReader::FileReader(GraphFile* file) : file_(file),
                                          neighborhoods_(nullptr),
                                          names_(nullptr) {
    return;
}

FileReader::FileReader(std::string filename) : file_(new GraphFile(filename)),
                                               neighborhoods_(nullptr),
                                               names_(nullptr) {
    return;
}

FileReader::~FileReader() {
    delete neighborhoods_;
    delete names_;
    return;
}

MatrixCellIntGraphFR::~MatrixCellIntGraphFR() {
    delete subset_family_;
    return;
}

////////////// data for graphs
//
AdjacencyLists* FileReader::TakeNeighborhoods() {
    AdjacencyLists* temp = nullptr;
    std::swap(temp, neighborhoods_);
    return temp;
}

VertexNames* FileReader::TakeNames() {
    VertexNames* temp = nullptr;
    std::swap(temp, names_);
    return temp;
}

LexTrie* MatrixCellIntGraphFR::TakeSubsetFamily() {
    LexTrie* temp = nullptr;
    std::swap(temp, subset_family_);
    return temp;
}

////////////// file processing
//

// encapsulates assert or die with error msg
inline void FileReader::AssertFormatOrDie(bool assertion,
                                          std::string errormsg) const {
    if (!assertion) {
        std::cerr << errormsg << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}

// FileReader for files in Dimacs Format:
// c This is a comment line
// p FORMAT NODES EDGES // we ignore FORMAT, NODES = number of nodes, etc.
// e U V // edge descriptors: (U,V) = e is an edge of G. not to be duplicated
// with e V U. U and V are integers in 1, 2, ..., n
void DimacsGraphFR::ReadFileOrDie() {
    bool problem_line_seen = false;
    size_t n = 0, m = 0;
    std::string line;
    std::set< std::pair<int, int> > edges;
    while (file_->GetLine(line)) {
        std::stringstream line_stream;
        std::string start_character, devnull;
        line_stream << line;
        line_stream >> start_character;
        if (start_character == "c") {
            continue;
        } else if (start_character == "p") {
            AssertFormatOrDie(!problem_line_seen, Dimacs_problem_line_count);
            problem_line_seen = true;
            line_stream << line;
            line_stream >> devnull;
            line_stream >> n;
            line_stream >> m;
        } else if (start_character == "e") {
            AssertFormatOrDie(problem_line_seen, Dimacs_problem_line_first);
            size_t u, v;
            line_stream << line;
            line_stream >> u;
            line_stream >> v;
            if (u < 1 || v < 1 || u > n || v > n || u == v) {
                // Die, invalid range
                std::cerr << "Dimacs graph file format error: invalid vertex";
                std::cerr << "range" << std::endl;
                exit(EXIT_FAILURE);
            }
            if ( u < v ) {
                std::swap(u, v);
            }
            std::pair<size_t, size_t> e = std::pair<size_t, size_t>(u, v);
            AssertFormatOrDie(edges.find(e) == edges.end(),
                              Dimacs_duplicate_edge);
            edges.insert(e);
        } else {
            AssertFormatOrDie(line.empty(), Dimacs_unknown_character);
        }
    }
    // initialize the adjacency lists for the graph
    neighborhoods_ = new AdjacencyLists;
    // create empty neighborhoods
    neighborhoods_->resize(n);
    // initialize vertex names
    names_ = new VertexNames;
    names_->resize(n);
    for (size_t i = 0; i < n; ++i) {
        std::stringstream ss;
        ss << i + 1;
        names_->operator[](i) = ss.str();
    }
    // get neighbors from edge set
    std::vector< std::list< size_t > > neighbors;
    neighbors.resize(n);
    for (std::pair<size_t, size_t> e : edges) {
        size_t u = e.first;
        size_t v = e.second;
        neighbors[u - 1].push_back(v - 1);
        neighbors[v - 1].push_back(u - 1);
    }

    // turn string representation into vertex representation
    for (size_t i = 0; i < n; ++i) {
        Vertices* nbhd = &neighborhoods_->operator[](i);
        for (size_t v : neighbors[i]) {
            nbhd->add(v);
        }
        std::sort(nbhd->begin(), nbhd->end());
    }
    return;
}

// FileReader for files in the following format:
// Line 1: non-negative integer denoting the number of vertices
// Line i>1: i-1, representing vertex i-1, followed by its neighbors as sorted
// integers delimited by whitespace
void SortedAdjacencyListFR::ReadFileOrDie() {
    std::string line, vertex, neighbor;
    std::stringstream line_stream;
    int order, line_number = 0;
    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;
    // process first line, consisting of the graphs order
    file_->GetLine(line);
    line_stream << line;  // grab line into stream
    line_stream >> order;  // parse piece up to whitespace
    AssertFormatOrDie(0 <= order, File_first_line_format);
    line_number++;
    line_stream.clear();
    line_stream.str("");  // reset line stream
    neighbor_names.resize(order);
    // initialize the adjacency lists for the graph
    neighborhoods_ = new AdjacencyLists;
    neighborhoods_->resize(order);   // create empty neighborhoods
    // initialize vertex names
    names_ = new VertexNames;
    names_->resize(order);
    int vertex_count = 0;
    while (file_->GetLine(line)) {
        AssertFormatOrDie(vertex_count < order, File_too_many_lines);
        line_stream << line;
        line_stream >> vertex;
        // assign name
        names_->operator[](vertex_count) = vertex;
        vertex_id[vertex] = vertex_count;
        while (line_stream >> neighbor) {
            neighbor_names[vertex_count].push_back(neighbor);
        }
        line_number++;
        vertex_count++;
        line_stream.clear();
        line_stream.str("");  // reset line stream
    }
    // turn string representation into vertex representation
    for (int i = 0; i < order; ++i) {
        Vertices* nbhd = &neighborhoods_->operator[](i);
        for (std::string neighbor : neighbor_names[i]) {
            Vertex v = vertex_id[neighbor];
            nbhd->add(v);
        }
        std::sort(nbhd->begin(), nbhd->end());
    }
    return;
}

void MatrixCellIntGraphFR::ReadFileOrDie() {
    std::string line, state;
    std::stringstream line_stream;
    int row_count = 0, col_count = 0, line_number = 0;
    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;
    // skip first line
    file_->GetLine(line);
    line_stream.clear();
    line_stream.str("");
    // second line: # of rows and # of columns
    file_->GetLine(line);
    line_stream << line;
    line_stream >> row_count;
    line_stream >> col_count;
    AssertFormatOrDie(0 <= row_count, Matrix_first_line_format);
    AssertFormatOrDie(0 <= col_count, Matrix_first_line_format);
    line_number++;
    line_stream.clear();
    line_stream.str("");  // reset line stream
    // extract matrix from file
    std::vector< std::vector< int > > matrix(row_count,
                                             std::vector< int >(col_count));
    int i = 0, j = 0, maxstate = 0;
    while (file_->GetLine(line)) {
        AssertFormatOrDie(i < row_count, Matrix_too_many_rows);
        j = 0;
        line_stream << line;
        while (line_stream >> state) {
            AssertFormatOrDie(j < col_count, Matrix_too_many_cols);
            if (state.compare("?") == 0 || state.compare("*") == 0 ||
               state.compare("-") == 0) {
                matrix[i][j] = kMissingData();
            } else {
                matrix[i][j] = atoi(state.c_str());
                maxstate = std::max(matrix[i][j], maxstate);
            }
            ++j;
        }
        line_stream.clear();
        line_stream.str("");  // reset line stream
        ++i;
    }
    ComputeGraphData(matrix, maxstate);
    return;
}

void MatrixCellIntGraphFR::ComputeGraphData(
        std::vector< std::vector< int > > matrix, int maxstate) {
    int row_count = matrix.size();
    int col_count = matrix[0].size(), cell_count = 0;
    subset_family_ = new LexTrie(row_count);
    std::map< const LexTrieNode*, Vertex > cell_id;
    std::map< std::string, Vertex> vertex_id;
    // subsets_[v] is the subset of the ground set for vertex v
    for (int j = 0; j < col_count; ++j) {
        std::vector< Subset > cells(maxstate + 1, Subset());
        for (int i = 0; i < row_count; ++i) {
            int state = matrix[i][j];
            if (state != kMissingData()) {
                cells[state].push_back(i);
            }
        }
        for (Subset& C : cells) {
            if (!C.empty()) {
                bool new_cell;
                std::sort(C.begin(), C.end());
                const LexTrieNode* node =
                        subset_family_->Insert< Subset >(C, &new_cell);
                if (new_cell) {
                    cell_id[node] = cell_count;
                    ++cell_count;
                    subsets_.push_back(Subset(C));
                    vertex_colors_.push_back(Multicolor());
                    int state = matrix[C[0]][j];
                    std::stringstream name;
                    name << j << '#' << state;
                    vertex_id[name.str()] = cell_count;
                }
                Vertex v = cell_id[node];
                vertex_colors_[v].push_back(j);
            }
        }
    }
    int order = cell_count;
    names_ = new VertexNames(order);
    for (auto p : vertex_id) {
        std::string name = p.first;
        Vertex v = p.second;
        names_->operator[](v - 1) = name;
    }

    neighborhoods_ = new AdjacencyLists(order);
    Vertex v = 0;

    std::vector< VertexVector > cells_of_taxon(row_count);
    for (Subset& C : subsets_) {
        for (Element t : C) {
            cells_of_taxon[t].push_back(v);
        }
        v++;
    }
    std::map< VertexPair, bool > edges;
    for (VertexVector V : cells_of_taxon) {
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
        neighborhoods_->operator[](u).add(v);
        neighborhoods_->operator[](v).add(u);
    }
    for (Vertices& V : *neighborhoods_) {
        std::sort(V.begin(), V.end());
    }
    return;
}

std::string NexusMRPFR::ParseParameter(std::string line,
                                       std::string parameter) const {
    std::string result;
    std::string delimeter_nospace = parameter + std::string("=");
    std::string delimeter_space = parameter + std::string(" = ");
    std::stringstream ss;
    if (line.find(delimeter_nospace) != std::string::npos) {
        std::string right_half = line.substr(line.find(delimeter_nospace) +
                                             delimeter_nospace.size());
        if (right_half[right_half.size() - 1] == ';') {
            right_half.erase(right_half.end() - 1);
        }
        ss << right_half;
        ss >> result;
    } else if (line.find(delimeter_space) != std::string::npos) {
        std::string right_half = line.substr(line.find(delimeter_space) +
                                             delimeter_space.size());
        if (right_half[right_half.size() - 1] == ';') {
            right_half.erase(right_half.end() - 1);
        }
        ss << right_half;
        ss >> result;
    } else {
        std::cerr << "Error in Nexus file format, cannot find '";
        std::cerr << delimeter_nospace << "' or '" << delimeter_space << "'";
        std::cerr << std::endl;
        exit(EXIT_FAILURE);
    }
    return result;
}

void NexusMRPFR::ReadFileOrDie() {
    std::string line, state;
    std::stringstream parameter;
    size_t row_count = 0, col_count = 0;
    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;
    // skip first two lines
    file_->GetLine(line);
    file_->GetLine(line);
    // third line: # of rows and # of columns
    file_->GetLine(line);
    parameter << ParseParameter(line, "ntax");
    parameter >> row_count;
    parameter.clear();
    parameter.str("");
    parameter << ParseParameter(line, "nchar");
    parameter >> col_count;
    // skip next two lines
    file_->GetLine(line);
    file_->GetLine(line);
    taxon_name_.resize(row_count);
    // extract matrix from file
    std::vector< std::vector< int > > matrix(row_count,
                                             std::vector< int >(col_count));
    size_t i = 0;
    while (file_->GetLine(line)) {
        if (line == std::string(";")) {
            break;
        }
        AssertFormatOrDie(i < row_count, Nexus_too_many_rows);
        std::stringstream line_stream;
        line_stream << line;
        line_stream >> taxon_name_[i];
        std::string row;
        line_stream >> row;
        for (size_t j = 0; j < row.size(); ++j) {
            AssertFormatOrDie(j < col_count, Nexus_too_many_cols);
            if (row[j] == '?' || row[j] == '*' || row[j] == '-') {
                matrix[i][j] = kMissingData();
            } else if (row[j] == '1') {
                matrix[i][j] = 1;
            } else if (row[j] == '0') {
                matrix[i][j] = 0;
            } else {
                AssertFormatOrDie(false, Nexus_unknown_symbol);
            }
        }
        ++i;
    }
    while (file_->GetLine(line)) {} // Finish processing input. For clarity of unit tests.
    ComputeGraphData(matrix, 1);
    return;
}

}  // namespace chordalg
