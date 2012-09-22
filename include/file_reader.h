#ifndef FILE_READER_H
#define FILE_READER_H

#include<cstdlib>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>

#include"chordalg_types.h"

namespace chordalg {

// Abstract class used to read files for graphs.
// To design a file reader class ClassFR, include the following:
//      1) Empty constructor ClassFR(std::string)
//
class FileReader
{
    public:
        virtual ~FileReader();

        AdjacencyLists* TakeNeighborhoods();
        VertexNameContainer* TakeNames();
    protected:
        FileReader(std::string);
        inline void AssertFormatOrDie(bool,std::string) const;

        AdjacencyLists* neighborhoods_;
        VertexNameContainer* names_;
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
// So this is ok:
// SortedAdjacencyListFR* my_fr = NewFileReader< SortedAdjacencyListFR >(std::string("file_name"));
//
// Compile error:
// FileReader* my_fr = NewFileReader< std::vector >(std::string("file_name"));
template< class FR >
FR* NewFileReader(std::string file_name)
{
    // ctor
    FR* fr_object = new FR(file_name);
    fr_object->ReadFileOrDie();

    // type_check voided to prevent compiler warning
    FileReader* type_check = fr_object; (void) type_check;
    return fr_object;
}

// FileReader for files in the following format:
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

}   // namespace chordalg

#endif // FILE_READER_H
