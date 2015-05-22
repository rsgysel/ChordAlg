#include "gtest/gtest.h"

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/potential_maxcliques.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

/////////////
// Frameworks

class IsPMCTest : public testing ::Test {
 public:
    void SetUp() {
        G_ = nullptr;
    }
    void TearDown() {
        delete G_;
    }
    void Read(const chordalg::AdjacencyLists& A,
              const chordalg::Vertices& K,
              bool is_pmc) {
        if (G_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            EXPECT_EQ(chordalg::PotentialMaxcliques::IsPMC(*G_, K), is_pmc);
        }
    }
 protected:
    chordalg::Graph* G_;
};  // IsPMCTest

class OneMoreVertexTest : public testing ::Test {
 public:
    void SetUp() {
        Gi_ = nullptr;
        Gi1_ = nullptr;
        MSi_ = nullptr;
        MSi1_ = nullptr;
        Pi_ = nullptr;
        Pi1_ = nullptr;
    }
    void TearDown() {
        delete Gi_;
        delete Gi1_;
        delete MSi_;
        delete MSi1_;
        delete Pi_;
        delete Pi1_;
    }
    void Read(const chordalg::AdjacencyLists& A,
              size_t pmc_count,
              const chordalg::PMCTrie* Pi = nullptr,
              const chordalg::MinsepTrie* MSi = nullptr,
              const chordalg::MinsepTrie* MSi1 = nullptr) {
        if (Gi_ || Gi1_ || MSi_ || MSi1_ || Pi_ || Pi1_) {
            FAIL() << "Use Read once in your test.";
        } else {
            Gi1_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            chordalg::Vertices V = Gi1_->V();
            chordalg::Vertex a = V.back();
            V.pop_back();
            Gi_ = chordalg::InducedSubgraph::New(Gi1_, V);
            Pi_ = Pi ? Pi : new chordalg::PMCTrie(Gi_->order());
            MSi_ = MSi ? MSi : new chordalg::MinsepTrie(Gi_->order());
            MSi1_ = MSi1 ? MSi1 : new chordalg::MinsepTrie(Gi1_->order());
            Pi1_ = chordalg::PotentialMaxcliques::OneMoreVertex(*Gi_, a, *Gi1_, *Pi_, *MSi_, *MSi1_);
            EXPECT_EQ(Pi1_->size(), pmc_count);
        }
    }
 protected:
    const chordalg::Graph* Gi_;
    const chordalg::Graph* Gi1_;
    const chordalg::MinsepTrie* MSi_;
    const chordalg::MinsepTrie* MSi1_;
    const chordalg::PMCTrie* Pi_;
    const chordalg::PMCTrie* Pi1_;
};  // OneMoreVertexTest

////////
// Tests

TEST_F(IsPMCTest, ManyMinsepsFourPMC) {
    Read(many_minseps_four, chordalg::Vertices({0, 2, 3, 4, 5}), true);
}

TEST_F(IsPMCTest, ManyMinsepsFourNotPMCFullComponents) {
    Read(many_minseps_four, chordalg::Vertices({0, 1}), false);
}

TEST_F(IsPMCTest, ManyMinsepsFourNotPMCNotClique) {
    Read(many_minseps_four, chordalg::Vertices({2, 3, 4, 5, 6, 7, 8, 9}), false);
}

TEST_F(IsPMCTest, TwoCliquesPMC) {
    Read(two_cliques, chordalg::Vertices({0, 1, 2, 3, 4}), true);
}

TEST_F(IsPMCTest, TwoCliquesSubclique) {
    Read(two_cliques, chordalg::Vertices({0, 1, 2}), false);
}

TEST_F(IsPMCTest, TwoCliquesDisconnected) {
    Read(two_cliques, chordalg::Vertices({0, 1, 3, 5, 6}), false);
}

TEST_F(OneMoreVertexTest, ManyMinsepsFourPMCToNewPMC) {
    chordalg::PMCTrie* P = new chordalg::PMCTrie(9);
    P->Insert(chordalg::Vertices({1, 6, 7, 8}));
    Read(many_minseps_four, 1, P);
}

TEST_F(OneMoreVertexTest, ManyMinsepsFourPMCNoNewPMC) {
    chordalg::PMCTrie* P = new chordalg::PMCTrie(9);
    P->Insert(chordalg::Vertices({1, 6, 7, 8}));
    Read(many_minseps_four, 1, P);
}

TEST_F(OneMoreVertexTest, ManyMinsepsFourMinsepAndNextVertexPMC) {
    chordalg::MinsepTrie* MS = new chordalg::MinsepTrie(9);
    MS->Insert(chordalg::Vertices({1, 5}));
    Read(many_minseps_four, 1, nullptr, nullptr, MS);
}

TEST_F(OneMoreVertexTest, ManyMinsepsFourCrossingMinseps) {
    chordalg::MinsepTrie* MS = new chordalg::MinsepTrie(9);
    // This minimal separator `contains a' (a = 9), does not generate new ones
    MS->Insert(chordalg::Vertices({6, 7, 8, 9}));
    // This minimal separator does not contain a, crosses the previous minimal
    // separator, and the intersection with each connected component (there are two)
    // give PMCs {1, 3, 7} and {1, 3, 6, 8, 9}
    MS->Insert(chordalg::Vertices({1, 3}));
    Read(many_minseps_four, 2, nullptr, nullptr, MS);
}
