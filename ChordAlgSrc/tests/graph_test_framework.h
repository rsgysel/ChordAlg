#ifndef GRAPH_TEST_FRAMEWORK_H_
#define GRAPH_TEST_FRAMEWORK_H_

#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

class GraphTest : public ::testing::Test {
  public:
    void SetUp() {
        G_ = nullptr;
    }
    void TearDown() {
        delete G_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if(G_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
        }
        return;
    }
  protected:
    chordalg::Graph* G_;
};  // GraphTest

#endif  // GRAPH_TEST_FRAMEWORK_H_
