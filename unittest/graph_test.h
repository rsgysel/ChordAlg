#ifndef GRAPH_TEST_H
#define GRAPH_TEST_H

#include <string>

#include "gtest/gtest.h"

#include "file_reader.h"
#include "graph.h"
#include "intersection_graph.h"

class GraphTest : public ::testing::Test {
 protected:
    chordalg::Graph* G;

    GraphTest() : G(NULL) {}
    virtual ~GraphTest() { delete G; }

    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string graph_dir(){
        return std::string("graphfiles/");}

    virtual void Read(std::string file_name) = 0;
};  // GraphTest

class SortedAdjacencyListTest : public GraphTest {
 protected:
    chordalg::SortedAdjacencyListFR* graph_file;

    SortedAdjacencyListTest() : graph_file(NULL) {}
    ~SortedAdjacencyListTest() { delete graph_file; return; }

    void Read(std::string file_name){
        delete graph_file;
        delete G;
        graph_file = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file_name);
        G = new chordalg::Graph(graph_file);
        return;
    }
};  // SortedAdjacencyListTest

class MatrixCellIntGraphTest : public GraphTest {
protected:
    chordalg::MatrixCellIntGraphFR* graph_file;

    MatrixCellIntGraphTest() : graph_file(NULL) {}
    ~MatrixCellIntGraphTest() { delete graph_file; return; }

    void Read(std::string file_name){
        delete graph_file;
        delete G;
        graph_file = chordalg::NewFileReader< chordalg::MatrixCellIntGraphFR >(file_name);
        G = new chordalg::ColoredIntersectionGraph(graph_file);
        return;
    }
};  // MatrixCellIntGraphTest

#endif // GRAPH_TEST_H

