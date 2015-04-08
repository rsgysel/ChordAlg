#include "gtest/gtest.h"

#include "ChordAlgSrc/atom_subgraphs.hpp"
#include "ChordAlgSrc/graph.h"
#include "graph_test_framework.h"

class AtomsTest : public GraphTest {
 public:
    void SetUp() {
        G_ = nullptr;
        A_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete A_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if(G_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            A_ = new chordalg::Atoms< chordalg::Graph >(*G_);
        }
        return;
    }
 protected:
    chordalg::Atoms< chordalg::Graph >* A_;
};  // AtomsTest

