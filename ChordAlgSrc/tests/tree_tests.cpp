#ifndef TREE_TEST_FRAMEWORK_H_
#define TREE_TEST_FRAMEWORK_H_

#include "gtest/gtest.h"

#include "ChordAlgSrc/chordalg_string.h"
#include "ChordAlgSrc/tree.h"
#include "ChordAlgSrc/vertices.h"
#include "test_graphs.h"

class TreeTest : public ::testing::Test {
 public:
    void SetUp() {
        T_ = nullptr;
    }
    void TearDown() {
        delete T_;
    }
    void Read(chordalg::AdjacencyLists& A) {
        if (T_) {
            FAIL() << "Use Read once in your test.";
        } else {
            T_ = new chordalg::Tree(new chordalg::AdjacencyLists(A));
        }
        return;
    }
 protected:
    chordalg::Tree* T_;
};  // TreeTest

TEST_F(TreeTest, TreeIsTree) {
    Read(tree);
    EXPECT_EQ(T_->IsTree(), true);
}

TEST_F(TreeTest, TwoCliquesNotTree) {
    Read(two_cliques);
    EXPECT_EQ(T_->IsTree(), false);
}

// Check that parenthesis are nested in Newick string
TEST_F(TreeTest, TreeNewickNestedParen) {
    Read(tree);
    int left_parens = 0;
    for (char c : T_->strNewick()) {
        switch (c) {
            case '(' : {
                ++left_parens;
                break;
            }
            case ')' : {
                EXPECT_GE(left_parens, 1);
                --left_parens;
                break;
            }
            default : {
                break;
            }
        }
    }
    EXPECT_EQ(left_parens, 0);
}

// Check that each vertex name appears exactly once in Newick string
TEST_F(TreeTest, TreeNewickVertexNames) {
    Read(tree);
    std::set< std::string > str_names;
    chordalg::StringTokens tokens = chordalg::Split(T_->strNewick(), "();,");
    for (auto tok : tokens) {
        str_names.insert(tok);
    }
    for (chordalg::Vertex v : *T_) {
        EXPECT_NE(str_names.find(T_->name(v)), str_names.end());
    }
    EXPECT_EQ(str_names.size(), T_->order());
}

#endif  // TREE_TEST_FRAMEWORK_H_
