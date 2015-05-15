#include "gtest/gtest.h"

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/separator_graph.h"
#include "test_graphs.h"

/////////////
// Frameworks

class SeparatorGraphTest : public testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        SepG_ = nullptr;
        A_ = nullptr;
    }
    void TearDown() {
        delete G_;
        delete SepG_;
        delete A_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (G_ || SepG_ || A_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            SepG_ = chordalg::SeparatorGraph::New(G_);
            A_ = chordalg::Atoms::New(G_);
        }
        return;
    }
    void CompareCrossingRelations() {
        size_t size = 0;
        for (auto H : *A_) {
            chordalg::SeparatorGraph* SepH = chordalg::SeparatorGraph::New(H);
            size += SepH->size();
            delete SepH;
        }
        EXPECT_EQ(size, SepG_->size());
    }
 protected:
    chordalg::Graph* G_;
    chordalg::SeparatorGraph* SepG_;
    chordalg::Atoms* A_;
};  // SeparatorGraphTest

////////
// Tests

TEST_F(SeparatorGraphTest, TreeXRel) {
    Read(tree);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, BipartiteReductionXRel) {
    Read(bipartite_reduction);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, xBipartiteReductionXRel) {
    Read(bipartite_reduction_x);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, stBipartiteReductionXRel) {
    Read(bipartite_reduction_st);
    CompareCrossingRelations();
}

TEST_F(SeparatorGraphTest, DimacsXRel) {
    Read(dimacs_test);
    CompareCrossingRelations();
}

// This integration test is brittle:
// `isomorphic' means that the graphs are isomorphic as
// graphs with labeled vertices. This is dependent on
// the order in which the minimal separators are read
// by the SeparatorGraph constructor

TEST_F(SeparatorGraphTest, CliqueAutomorphism) {
    Read(clique);
    for (auto H : *A_) {
        chordalg::SeparatorGraph* SepH = chordalg::SeparatorGraph::New(H);
        EXPECT_EQ(SepH->IsIsomorphic(SepG_), true);
        delete SepH;
    }
}
