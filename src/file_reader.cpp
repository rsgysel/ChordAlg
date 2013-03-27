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
    return;
}

FileReader::~FileReader(){
    file_stream_.close();
    delete neighborhoods_;
    delete names_;
    return;
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
    std::string line, vertex, neighbor;
    std::stringstream line_stream;
    int order, line_number = 0;

    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;

    // format error messages
    const std::string first_line_format("Format Error: First line must be a non-negative integer denoting the number of vertices\n"),
        first_int_format("Format Error: Line i must start with i-2 for i > 1\n"),
        too_many_lines("Format Error: Too many lines; first line specifies number of lines (vertices)\n");

    // process first line, consisting of the graphs order
    getline(file_stream_, line); line_stream << line; // grab line into stream
	line_stream >> order; // parse piece up to whitespace
	AssertFormatOrDie( 0 <= order, first_line_format );
	line_number++;
    line_stream.clear(); line_stream.str("");   // reset line stream

    neighbor_names.resize(order);

    // initialize the adjacency lists for the graph
    neighborhoods_ = new AdjacencyLists;
    neighborhoods_->resize(order);   // create empty neighborhoods

    // initialize vertex names
    names_ = new VertexNameContainer;
    names_->resize(order);

    int vertex_count = 0;
    while( getline(file_stream_, line) )
    {
        line_stream << line;

        line_stream >> vertex;
        // assign name
        (*names_)[vertex_count] = vertex;
        vertex_id[vertex] = vertex_count;

        while(line_stream >> neighbor)
            neighbor_names[vertex_count].push_back(neighbor);

        AssertFormatOrDie(vertex_count <= order, too_many_lines);
        line_number++;
        vertex_count++;

        line_stream.clear(); line_stream.str("");   // reset line stream
    }

    // turn string representation into vertex representation
    for( int i = 0; i < order; ++i )
    {
        std::vector< Vertex > neighborhood;
        for( std::string neighbor : neighbor_names[i] )
        {
            Vertex v = vertex_id[neighbor];
            neighborhood.push_back(v);
        }
        std::sort(neighborhood.begin(), neighborhood.end());
        neighborhood.swap((*neighborhoods_)[i]);
    }

    file_stream_.close();
    return;
}

}   // namespace chordalg
