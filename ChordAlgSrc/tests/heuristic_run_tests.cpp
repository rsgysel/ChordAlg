#include "gtest/gtest.h"

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/triangulation.h"
#include "test_graphs.h"

class HeuristicRunTest : public ::testing::Test {
 public:
    void SetUp() {
        G_ = nullptr;
        R_ = nullptr;
        H_ = nullptr;
    }
    void TearDown() {
        delete G_;
        for (auto P : EP_) {
            delete P;
        }
        delete R_;
        delete H_;
    }
    void Read(chordalg::AdjacencyLists& A,
              chordalg::EliminationMode mode,
              chordalg::EliminationCriterion criterion,
              size_t runs=1,
              bool atoms=false,
              float deficiency_wt=1,
              float separation_wt=1) {
        if (G_ || R_ || H_) {
            FAIL() << "Use Read once in your test.";
        } else {
            G_ = new chordalg::Graph(new chordalg::AdjacencyLists(A));
            EP_.push_back(new chordalg::EliminationParameters(
                criterion, mode, deficiency_wt, separation_wt));
            R_ = new chordalg::HeuristicRun(G_, &EP_, atoms, runs);
            R_->Run();
            H_ = chordalg::Triangulation::New(G_, R_);
        }
    }   
 protected:
    chordalg::Graph* G_;
    std::vector< chordalg::EliminationParameters* > EP_;
    chordalg::HeuristicRun* R_;
    chordalg::Triangulation* H_;
};  // HeuristicRunTest

TEST_F(HeuristicRunTest, BipartiteReduction) {
    Read(bipartite_reduction,
         chordalg::EliminationMode::CLASSIC,
         chordalg::EliminationCriterion::DEFICIENCY);
    EXPECT_EQ(H_->IsTriangulated(), true);
}

TEST_F(HeuristicRunTest, ManyMinsepsFour) {
    Read(many_minseps_four,
         chordalg::EliminationMode::CLASSIC,
         chordalg::EliminationCriterion::DEFICIENCY);
    EXPECT_EQ(H_->IsTriangulated(), true);
}
