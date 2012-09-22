#ifndef GRAPH_TEST_H
#define GRAPH_TEST_H

#include<string>

#ifdef TESTS_ON

#include "gtest/gtest.h"

#include"file_reader.h"
#include"graph.h"

class SortedAdjacencyListFRTest : public ::testing::Test {
 protected:
    chordalg::Graph* G;
    chordalg::SortedAdjacencyListFR* graph_file;

    SortedAdjacencyListFRTest() : G(NULL), graph_file(NULL) {}

    virtual ~SortedAdjacencyListFRTest() {
        delete graph_file;
        delete G;
    }

    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string graph_dir(){return std::string("C:/Users/rob/Desktop/Dropbox/My Research/Code and Executables (Dist.)/dev/ChordAlg/graphfiles/");}

    virtual void Read(std::string file_name){
        delete graph_file;
        delete G;
        graph_file = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file_name);
        G = new chordalg::Graph(*graph_file);
    }
};

#endif // TEST_ON

#endif // GRAPH_TEST_H
