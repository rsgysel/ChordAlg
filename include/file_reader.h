#ifndef FILE_READER_H
#define FILE_READER_H

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "chordalg_types.h"
#include "lex_trie.h"
#include "utilities.h"
#include "vertex_utilities.h"

namespace chordalg {

// Abstract class used to read files for graphs.
// To design a file reader class ClassFR, include the following:
//      1) private constructor ClassFR(std::string) that initializes FileReader with input string
//      2) private ReadFileOrDie() method. you must close file_stream_ here
//      3) friend access to factory method template< class FR > friend FR* NewFileReader(std::string);
class FileReader
{
    public:
        virtual ~FileReader();

        AdjacencyLists* TakeNeighborhoods();
        VertexNames* TakeNames();
    protected:
        FileReader(std::string);
        inline void AssertFormatOrDie(bool,std::string) const;

        AdjacencyLists* neighborhoods_;
        VertexNames* names_;
        std::ifstream file_stream_;
    private:
        FileReader(){};   // default constructor disabled

        // construction and initialization deferred to NewFileReader
        template< class FR > friend FR* NewFileReader(std::string);
        virtual void ReadFileOrDie() = 0;
};  // FileReader

// FileReader factory construction
// FR must be derived from FileReader
//
template< class FR >
FR* NewFileReader( std::string file_name )
{
    // ctor
    FR* fr_object = new FR( file_name );
    fr_object->ReadFileOrDie();

    // type check FR. voided to prevent compiler warning
    FileReader* type_check = fr_object; ( void ) type_check;
    return fr_object;
}

// FileReader for sorted adjacency list (.sal) files having format:
//      Line 1: non-negative integer denoting the number of vertices
//      Line i>1: i-1, representing vertex i-1, followed by its neighbors as sorted integers delimited by whitespace
class SortedAdjacencyListFR : public FileReader {
    public:
        ~SortedAdjacencyListFR(){};
    private:
        template< class FR > friend FR* NewFileReader(std::string);
        SortedAdjacencyListFR(std::string file_name) : FileReader(file_name) {};
        void ReadFileOrDie();
};  // SortedAdjacencyListFR

class MatrixCellIntGraphFR : public FileReader
{
    public:
        ~MatrixCellIntGraphFR(){};

        LexTrie* TakeSubsetFamily();
        std::vector< Subset > subsets() const { return subsets_; }
        std::vector< Multicolor > vertex_colors() const { return vertex_colors_; }
    private:
        LexTrie* subset_family_;
        std::vector< Subset > subsets_;
        std::vector< Multicolor > vertex_colors_;

        template< class FR > friend FR* NewFileReader( std::string );

        MatrixCellIntGraphFR( std::string file_name ) : FileReader( file_name ) {};
        void ReadFileOrDie();

        int kMissingData(){ return -1; }
        void ComputeGraphData( std::vector< std::vector< int > >, int );
};  // MatrixCellIntGraphFR

}   // namespace chordalg

#endif // FILE_READER_H
