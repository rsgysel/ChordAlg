#include "file_reader.h"

namespace chordalg {

////////////// ctor & dtors
//
FileReader::FileReader( std::string filename ) : neighborhoods_( NULL ), names_( NULL )
{
    file_stream_.open( filename.c_str() );
    if( !file_stream_ ) // exit on error
    {
        std::cerr << "Error opening file " << filename << "." << std::endl;
        exit( EXIT_FAILURE );
    }
    return;
}

FileReader::~FileReader()
{
    file_stream_.close();
    delete neighborhoods_;
    delete names_;
    return;
}

////////////// data for graphs
//
AdjacencyLists* FileReader::TakeNeighborhoods()
{
    AdjacencyLists* temp = neighborhoods_;
    neighborhoods_ = NULL;
    return temp;
}

VertexNameContainer* FileReader::TakeNames()
{
    VertexNameContainer* temp = names_;
    names_ = NULL;
    return temp;
}

LexTrie* MatrixCellIntGraphFR::TakeSubsetFamily()
{
    LexTrie* temp = subset_family_;
    subset_family_ = NULL;
    return temp;
}

////////////// file processing
//

// encapsulates assert or die with error msg
inline void FileReader::AssertFormatOrDie(bool assertion, std::string errormsg) const
{
    if( !assertion )
    {
        std::cerr << errormsg;
        exit( EXIT_FAILURE );
    }
    return;
}

// FileReader for files in the following format:
//      Line 1: non-negative integer denoting the number of vertices
//      Line i>1: i-1, representing vertex i-1, followed by its neighbors as sorted integers delimited by whitespace
void SortedAdjacencyListFR::ReadFileOrDie()
{
    std::string line, vertex, neighbor;
    std::stringstream line_stream;
    int order, line_number = 0;

    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;

    // format error messages
    const std::string first_line_format( "Format Error: First line must be a non-negative integer denoting the number of vertices\n" ),
        first_int_format( "Format Error: Line i must start with i-2 for i > 1\n" ),
        too_many_lines( "Format Error: Too many lines; first line specifies number of lines (vertices)\n" );

    // process first line, consisting of the graphs order
    getline( file_stream_, line ); line_stream << line; // grab line into stream
	line_stream >> order; // parse piece up to whitespace
	AssertFormatOrDie( 0 <= order, first_line_format );
	line_number++;
    line_stream.clear(); line_stream.str( "" );   // reset line stream

    neighbor_names.resize( order );

    // initialize the adjacency lists for the graph
    neighborhoods_ = new AdjacencyLists;
    neighborhoods_->resize( order );   // create empty neighborhoods

    // initialize vertex names
    names_ = new VertexNameContainer;
    names_->resize( order );

    int vertex_count = 0;
    while( getline( file_stream_, line ) )
    {
        line_stream << line;

        line_stream >> vertex;
        // assign name
        names_->operator[]( vertex_count ) = vertex;
        vertex_id[ vertex ] = vertex_count;

        while( line_stream >> neighbor )
            neighbor_names[ vertex_count ].push_back( neighbor );

        AssertFormatOrDie( vertex_count <= order, too_many_lines );
        line_number++;
        vertex_count++;

        line_stream.clear(); line_stream.str( "" );   // reset line stream
    }

    // turn string representation into vertex representation
    for( int i = 0; i < order; ++i )
    {
        std::vector< Vertex > neighborhood;
        for( std::string neighbor : neighbor_names[ i ] )
        {
            Vertex v = vertex_id[ neighbor ];
            neighborhood.push_back( v );
        }
        std::sort( neighborhood.begin(), neighborhood.end() );
        neighborhood.swap( neighborhoods_->operator[]( i ) );
    }

    file_stream_.close();
    return;
}

void MatrixCellIntGraphFR::ReadFileOrDie()
{
    std::string line, state;
    std::stringstream line_stream;
    int row_count = 0, col_count = 0, line_number = 0;

    std::map< std::string, Vertex > vertex_id;
    std::vector< std::list< std::string > > neighbor_names;

    // format error messages
    const std::string first_line_format( "Format Error: First line must be two non-negative integer denoting the number of rows and columns\n" ),
        too_many_cols( "Format Error: Too many columns\n" ),
        too_many_rows( "Format Error: Too many rows\n" );

    // skip first line
    getline( file_stream_, line );
    line_stream.clear(); line_stream.str( "" );

    // second line: # of rows and # of columns
    getline( file_stream_, line );
    line_stream << line;
	line_stream >> row_count;
	line_stream >> col_count;
	AssertFormatOrDie( 0 <= row_count, first_line_format );
	AssertFormatOrDie( 0 <= col_count, first_line_format );
	line_number++;
    line_stream.clear(); line_stream.str( "" );   // reset line stream

    // extract matrix from file
    std::vector< std::vector< int > > matrix( row_count, std::vector< int >( col_count ) );
    int i = 0, j = 0, maxstate = 0;
    while( getline( file_stream_, line ) )
    {
        AssertFormatOrDie( i < row_count, too_many_rows );
        j = 0;
        line_stream << line;
        while( line_stream >> state )
        {
            AssertFormatOrDie( j < col_count, too_many_cols );
            if( state.compare( "?" ) == 0 || state.compare( "*" ) == 0 )
                matrix[ i ] [ j ] = kMissingData();
            else
            {
                matrix[ i ][ j ] = atoi( state.c_str() );
                maxstate = std::max( matrix[ i ][ j ], maxstate );
            }
            ++j;
        }
        line_stream.clear(); line_stream.str( "" );   // reset line stream
        ++i;
    }
    file_stream_.close();

    ComputeGraphData( matrix, maxstate );
    return;
}

void MatrixCellIntGraphFR::ComputeGraphData( std::vector< std::vector< int > > matrix, int maxstate )
{
    int row_count = matrix.size(), col_count = matrix[ 0 ].size(), cell_count = 0;
    subset_family_ = new LexTrie( row_count );

    std::map< const LexTrieNode*, Vertex > cell_id;

    // Create subsets_. subsets_[v] is the subset of the ground set related to vertex v
    for( int j = 0; j < col_count; ++j )
    {
        std::vector< Subset > cells( maxstate + 1, Subset() );
        for( int i = 0; i < row_count; ++i )
        {
            int state = matrix[ i ][ j ];
            if( state != kMissingData() )
                cells[ state ].push_back( i );
        }

        for( Subset& C : cells )
        {
            if( !C.empty() )
            {
                bool new_cell;
                std::sort( C.begin(), C.end() );
                const LexTrieNode* node = subset_family_->Insert< Subset, Subset::const_iterator >( C, new_cell );
                if( new_cell )
                {
                    cell_id[ node ] = cell_count;
                    ++cell_count;
                    subsets_.push_back( Subset( C ) );
                    vertex_colors_.push_back( Multicolor() );
                }
                Vertex v = cell_id[ node ];
                vertex_colors_[ v ].push_back( j );
            }
        }
    }
    int order = cell_count;

    names_ = new VertexNameContainer( cell_count );
    for( int i = 0; i < cell_count; ++i )
    {
        std::stringstream i_str;
        i_str << i;
        names_->operator[]( i ) = i_str.str();
    }

    neighborhoods_ = new AdjacencyLists( order );
    Vertex v = 0;
    std::vector< VertexContainer > cells_of_taxon( row_count );
    for( Subset& C : subsets_ )
    {
        for( Element t : C )
            cells_of_taxon[t].push_back( v );
        v++;
    }
    std::map< VertexPair, bool > edges;
    for( VertexContainer V : cells_of_taxon )
    {
        for( Vertex v : V )
        {
            for( Vertex u : V )
            {
                if( u < v )
                    edges[ VertexPair( u, v ) ] = true;
            }
        }
    }
    for( std::pair< VertexPair, bool > p : edges )
    {
        VertexPair e = p.first;
        Vertex u = e.first, v = e.second;
        neighborhoods_->operator[]( u ).push_back( v );
        neighborhoods_->operator[]( v ).push_back( u );
    }
    for( VertexContainer& V : *neighborhoods_ )
        std::sort( V.begin(), V.end() );

    return;
}

}   // namespace chordalg
