#include "file_reader.h"

namespace chordalg {

////////////// ctor & dtors
//
FileReader::FileReader(std::string filename) : neighborhoods_(NULL), names_(NULL){
    file_stream_.open(filename.c_str());
    if(!file_stream_) // exit on error
    {
        std::cerr << "Error opening file " << filename << "." << std::endl;
        exit(EXIT_FAILURE);
    }
}

FileReader::~FileReader(){
    file_stream_.close();
    delete neighborhoods_;
    delete names_;
}

////////////// data for graphs
//
AdjacencyLists* FileReader::TakeNeighborhoods(){
    AdjacencyLists* temp = neighborhoods_;
    neighborhoods_ = NULL;
    return temp;
}

VertexNameContainer* FileReader::TakeNames(){
    VertexNameContainer* temp = names_;
    names_ = NULL;
    return temp;
}

////////////// file processing
//

// encapsulates assert or die with error msg
inline void FileReader::AssertFormatOrDie(bool assertion, std::string errormsg) const {
    if(!assertion)
    {
        std::cerr << errormsg;
        exit(EXIT_FAILURE);
    }
    return;
}

// FileReader for files in the following format:
//      Line 1: non-negative integer denoting the number of vertices
//      Line i>1: i-1, representing vertex i-1, followed by its neighbors as sorted integers delimited by whitespace
void SortedAdjacencyListFR::ReadFileOrDie(){
    std::string line;
    std::stringstream line_stream;
    int order, vertex, neighbor, line_number = 0;

    // format error messages
    const std::string first_line_format("Format Error: First line must be a non-negative integer denoting the number of vertices\n"),
        first_int_format("Format Error: Line i must start with i-2 for i > 1\n"),
        too_many_lines("Format Error: Too many lines; first line specifies number of lines (vertices)\n");

    // process first line, consisting of the graphs order
    getline(file_stream_, line); line_stream << line; // grab line into stream
	line_stream >> order; // parse piece up to whitespace
	AssertFormatOrDie(0 <= order, first_line_format);
	line_number++;
    line_stream.clear(); line_stream.str("");   // reset line stream

    // initialize the adjacency lists for the graph
    neighborhoods_ = new AdjacencyLists;
    neighborhoods_->resize(order);   // create empty neighborhoods

    // initialize vertex names
    names_ = new VertexNameContainer;
    names_->resize(order);

    // buffer used to store one adjacency list at a time, one per file line
    VertexContainer vertex_buffer;

    while( getline(file_stream_, line) )
    {
        // put neighborhood of vertex in buffer
        vertex_buffer.reserve(order);
        line_stream << line;

        line_stream >> vertex;
        AssertFormatOrDie(vertex == line_number - 1, first_int_format);
        AssertFormatOrDie(vertex <= order, too_many_lines);

        while(line_stream >> neighbor)
            vertex_buffer.push_back(neighbor);

        // assign neighborhood to neighborhoods_
        vertex_buffer.swap((*neighborhoods_)[vertex]);  // capacity is also swapped, so there is some waste here

        // assign name (in this format, naming is trivial)
        (*names_)[vertex] = vertex;

        // assign name to names_
        line_number++;

        line_stream.clear(); line_stream.str("");   // reset line stream
    }

    file_stream_.close();
    return;
}

}   // namespace chordalg
