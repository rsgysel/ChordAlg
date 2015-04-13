#include <set>
#include <string>
#include <sstream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ChordAlgSrc/lex_trie.h"

/////////////
// Frameworks

class LexTrieTest : public ::testing::Test {
  public:
    void SetUp() {
        T_ = nullptr;
        new_insert_ = false;
    }
    void TearDown() {
        delete T_;
    }
    void InitTrie(size_t n) {
        if(T_) {
            FAIL() << "Use InitTrie once in your test.";
        } else {
            T_ = new chordalg::LexTrie(n);
        }
    }
    void Insert(std::initializer_list<size_t> X) {
        T_->SortedInsert< std::initializer_list<size_t> >(X, &new_insert_);
        return;
    }
    void InsertSetAndString(std::initializer_list<size_t> X) {
        Insert(X);
        if(X.size() == 0) {
            T_strings.insert(std::string());
        } else {
            std::ostringstream oss;
            std::copy(X.begin(), X.end() - 1, std::ostream_iterator<size_t>(oss, " "));
            oss << *(X.end() - 1);
            T_strings.insert(oss.str());
        }
        return;
    }
    bool IsTrieStringSet() const {
        return IsTrieInStringSet() && (T_->size() == T_strings.size());
    }
    bool IsTrieInStringSet() const {
        for(auto X : *T_) {
            if(T_strings.find(X.str()) == T_strings.end()) {
                return false;
            }
        }
        return true;
    }
  protected:
    chordalg::LexTrie* T_;
    std::set< std::string > T_strings;
    bool new_insert_;
};  // LexTrieTest

////////
// Tests

TEST_F(LexTrieTest, Empty) {
    this->InitTrie(10);
    EXPECT_EQ(T_->size(), 0);
}

TEST_F(LexTrieTest, Insertion) {
    this->InitTrie(10);
    this->Insert({2, 4, 9});
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(T_->size(), 1);
}

TEST_F(LexTrieTest, SuperfluousInsertion) {
    this->InitTrie(10);
    this->Insert({2, 4, 9});
    EXPECT_EQ(new_insert_, true);
    this->Insert({2, 4, 9});
    EXPECT_EQ(new_insert_, false);
    EXPECT_EQ(T_->size(), 1);
}

// Test internal node branching
TEST_F(LexTrieTest, InternalBranch) {
    this->InitTrie(10);
    this->InsertSetAndString({2, 4, 9});  
    EXPECT_EQ(new_insert_, true);
    this->InsertSetAndString({2, 3, 5});
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieStringSet(), true);
}

// Test internal node set indicators
TEST_F(LexTrieTest, InternalSet) {
    this->InitTrie(10);
    this->InsertSetAndString({2, 4});
    EXPECT_EQ(new_insert_, true);
    this->InsertSetAndString({2});          // Internal to existing branch.
    EXPECT_EQ(new_insert_, true);
    this->InsertSetAndString({2, 4, 9});    // Extend existing branch.
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieStringSet(), true);
    EXPECT_EQ(T_->size(), 3);
}

// Test root branching
TEST_F(LexTrieTest, RootBranch){
    this->InitTrie(10);
    this->InsertSetAndString({2, 4, 9});
    EXPECT_EQ(new_insert_, true);
    this->InsertSetAndString({1, 6, 7, 8});
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieStringSet(), true);
    EXPECT_EQ(T_->size(), 2);  
}

