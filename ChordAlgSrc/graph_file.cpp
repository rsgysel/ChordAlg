#include "ChordAlgSrc/graph_file.h"

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "ChordAlgSrc/chordalg_string.h"

namespace chordalg {

GraphFile::GraphFile(std::string filename) :
    file_type_(FileType::UNKNOWN) {
    file_stream_.open(filename.c_str());
    AssertOrDie(file_stream_, "Error: can't open " + filename);
    GetFileType(filename);
    return;
}

GraphFile::GraphFile() :
    file_type_(FileType::UNKNOWN) {
    return;
}

GraphFile::GraphFile(FileType file_type) :
    file_type_(file_type) {
    return;
}

GraphFile::~GraphFile() {
    file_stream_.close();
    return;
}

std::istream& GraphFile::GetLine(std::string& str) {
    return std::getline(file_stream_, str);
}

void GraphFile::AssertOrDie(bool assertion, std::string error) {
    if (!assertion) {
        std::cerr << error << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}

bool GraphFile::IsTwoStateTaxon(std::string taxon, char missing_char) {
    for (char c : taxon) {
        if (c != '0' && c != '1' && c != missing_char) {
            return false;
        }
    }
    return true;
}

void GraphFile::AssignFileType(FileType file_type) {
    AssertOrDie(file_type_ == FileType::UNKNOWN,
                 "GraphFile::AssignFileType ambiguous filetype");
    file_type_ = file_type;
    return;
}

void GraphFile::GetFileType(std::string filename) {
    if (IsAdjacencyListFile(filename)) {
        AssignFileType(FileType::ADJLIST);
    } else if (IsDimacsFile(filename)) {
        AssignFileType(FileType::DIMACS);
    } else if (IsMatrixFile(filename)) {
        AssignFileType(FileType::CHARACTERMATRIX);
    } else if (IsNexusMRPFile(filename)) {
        AssignFileType(FileType::NEXUSMRP);
    } else {
        AssertOrDie(false, "Error: " + filename + " format unrecognized");
    }
    return;
}

bool GraphFile::IsAdjacencyListFile(std::string filename) const {
    std::ifstream file_stream;
    file_stream.open(filename);
    AssertOrDie(file_stream, "Error: can't open " + filename);

    // First line should be number of vertices
    std::string line;
    getline(file_stream, line);
    if (!IsNum(line)) {
        return false;
    }
    size_t order = std::stoi(line);

    // Next lines should be adjacency lists
    size_t line_number = 0;
    std::set< StringPair > edges_seen;
    while (std::getline(file_stream, line)) {
        ++line_number;
        if (line_number > order) {
            return false;
        }
        StringTokens line_tokens = Split(line, " \t");
        if (line_tokens.empty()) {
            return false;
        }
        std::string vertex = line_tokens[0];
        for (size_t i = 1; i < line_tokens.size(); ++i) {
            StringPair edge(vertex, line_tokens[i]);
            if (edges_seen.find(edge) != edges_seen.end()) {
                return false;
            }
            edges_seen.insert(edge);
        }
    }
    if (line_number != order) {
        return false;
    }

    // Check edge symmetry
    for (auto e : edges_seen) {
        StringPair f(e.second, e.first);
        if (edges_seen.find(f) == edges_seen.end()) {
            return false;
        }
    }
    file_stream.close();
    return true;
}

bool GraphFile::IsDimacsFile(std::string filename) const {
    std::ifstream file_stream;
    file_stream.open(filename);
    AssertOrDie(file_stream, "Error: can't open " + filename);

    bool problem_line_seen = false;
    size_t vertices = 0, edges = 0;
    std::set< StringPair > edges_seen;
    std::set< std::string > vertices_seen;
    std::string line;
    while (std::getline(file_stream, line)) {
        StringTokens line_tokens = Split(line, " \t");
        if (line_tokens.empty() || line_tokens[0].length() != 1) {
            return false;
        }
        switch (line_tokens[0][0]) {
            case 'c': {
                break;
            }
            case 'e' : {
                if (!problem_line_seen ||
                    line_tokens.size() != 3 ||
                    !IsNum(line_tokens[1]) ||
                    !IsNum(line_tokens[2])) {
                    return false;
                }
                std::string u = line_tokens[1], v = line_tokens[2];
                if (edges_seen.find(StringPair(u, v)) != edges_seen.end()) {
                    return false;
                }
                vertices_seen.insert(u);
                vertices_seen.insert(v);
                edges_seen.insert(StringPair(u, v));
                edges_seen.insert(StringPair(v, u));
                break;
            }
            case 'p' : {
                if (problem_line_seen ||
                    line_tokens.size() != 4 ||
                    !IsNum(line_tokens[2]) ||
                    !IsNum(line_tokens[3]) ) {
                    return false;
                }
                problem_line_seen = true;
                vertices = std::stoi(line_tokens[2]);
                edges = std::stoi(line_tokens[3]);
                break;
            }
            default: {
                return false;
            }
        }
    }
    if (2*edges != edges_seen.size() ||
        vertices > vertices_seen.size() ||
        !problem_line_seen) {
        return false;
    }

    file_stream.close();
    return true;
}

bool GraphFile::IsMatrixFile(std::string filename) const {
    std::ifstream file_stream;
    file_stream.open(filename);
    AssertOrDie(file_stream, "Error: can't open " + filename);

    // First line is the name of the experiment, and is arbitrary
    std::string line;
    getline(file_stream, line);

    // Second line is the number of rows and columns
    std::getline(file_stream, line);
    StringTokens line_tokens = Split(line, " \t");
    if (line_tokens.size() != 2 ||
        !IsNum(line_tokens[0]) ||
        !IsNum(line_tokens[1])) {
        return false;
    }
    size_t rows = std::stoi(line_tokens[0]), cols = std::stoi(line_tokens[1]);

    size_t row_number = 0;
    while (std::getline(file_stream, line)) {
        ++row_number;
        if (row_number > rows) {
            return false;
        }
        line_tokens = Split(line, " \t");
        if (line_tokens.size() != cols) {
            return false;
        }
        for (std::string token : line_tokens) {
            if (!IsNum(token) &&
                token.compare("?") != 0 &&
                token.compare("*") != 0) {
                return false;
            }
        }
    }
    if (row_number != rows) {
        return false;
    }

    file_stream.close();
    return true;
}

bool GraphFile::IsNexusMRPFile(std::string filename) const {
    std::ifstream file_stream;
    file_stream.open(filename);
    AssertOrDie(file_stream, "Error: can't open " + filename);

    // Line 1: #NEXUS
    std::string line;
    std::getline(file_stream, line);
    StringTokens line_tokens = Split(line, " \t");
    if (line_tokens.size() != 1 ||
        line_tokens[0].compare("#nexus") != 0) {
        return false;
    }

    // Line 2: Begin Data;
    do {
        std::getline(file_stream, line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    line_tokens = Split(line, " \t;");
    if (line_tokens.size() != 2 ||
        line_tokens[0].compare("begin") != 0 ||
        line_tokens[1].compare("data") != 0) {
        return false;
    }

    // Line 3: Dimensions ntax = 3, nchar = 3;
    do {
        std::getline(file_stream, line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    line_tokens = Split(line, " \t=;");
    if (line_tokens.size() != 5 ||
        line_tokens[0].compare("dimensions") != 0 ||
        line_tokens[1].compare("ntax") != 0 ||
        !IsNum(line_tokens[2]) ||
        line_tokens[3].compare("nchar") != 0 ||
        !IsNum(line_tokens[4])) {
        return false;
    }
    size_t rows = std::stoi(line_tokens[2]);
    size_t cols = std::stoi(line_tokens[4]);

    // Line 4: Format datatype=standard symbols="01" Missing=?;
    do {
        std::getline(file_stream, line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    line_tokens = Split(line, " \t=;");
    if (line_tokens.size() != 7 ||
        line_tokens[0].compare("format") != 0 ||
        line_tokens[1].compare("datatype") != 0 ||
        line_tokens[2].compare("standard") != 0 ||
        line_tokens[3].compare("symbols") != 0 ||
        line_tokens[4].compare("\"01\"") != 0 ||
        line_tokens[5].compare("missing") != 0 ||
        line_tokens[6].size() != 1) {
        return false;
    }
    char missing_char = line_tokens[6][0];

    // Line 5: Matrix
    do {
        std::getline(file_stream, line);
        line_tokens = Split(line, " \t");
    } while (line_tokens.empty());
    line_tokens = Split(line, " \t");
    if (line_tokens.size() != 1 ||
        line_tokens[0].compare("matrix")) {
        return false;
    }

    // Matrix lines: taxon_name 0101?
    size_t row = 0;
    while (std::getline(file_stream, line) &&
           line.compare(";") != 0) {
        ++row;
        line_tokens = Split(line, " \t");
        if (line_tokens.size() != 2 ||
            line_tokens[1].length() != cols ||
            row > rows ||
            !IsTwoStateTaxon(line_tokens[1], missing_char)) {
            return false;
        }
    }

    // Last line: end;
    std::getline(file_stream, line);
    line_tokens = Split(line, " \t;");
    if (line_tokens.size() != 1 ||
        line_tokens[0].compare("end") != 0) {
        return false;
    }

    // Whitespace
    while (std::getline(file_stream, line)) {
        line_tokens = Split(line, " \t");
        if (!line_tokens.empty()) {
            return false;
        }
    }

    file_stream.close();
    return true;
}

}  // namespace chordalg
