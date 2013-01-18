#ifndef GRAPH_TEST_H
#define GRAPH_TEST_H

#include <string>

#include "gtest/gtest.h"

#include "file_reader.h"
#include "graph.h"

class SortedAdjacencyListTest : public ::testing::Test {
 protected:
    chordalg::Graph* G;
    chordalg::SortedAdjacencyListFR* graph_file;

    SortedAdjacencyListTest() : G(NULL), graph_file(NULL) {}

    virtual ~SortedAdjacencyListTest() {
        delete graph_file;
        delete G;
    }

    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string graph_dir(){
        return std::string("graphfiles/");}

    virtual void Read(std::string file_name){
        delete graph_file;
        delete G;
        graph_file = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file_name);
        G = new chordalg::Graph(graph_file);
    }
};

#endif // GRAPH_TEST_H

