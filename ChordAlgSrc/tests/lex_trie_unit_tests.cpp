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
    void Insert(chordalg::Subset X) {
        T_->Insert< chordalg::Subset >(X, &new_insert_);
        return;
    }
    void InsertAndSerialize(chordalg::Subset X) {
        Insert(X);
        T_Serialization.insert(Serialize(X));
        return;
    }
    bool IsTrieSerializedSet() const {
        return IsTrieInSerializedSet() && (T_->size() == T_Serialization.size());
    }
    bool IsTrieInSerializedSet() const {
        for(auto X : *T_) {
            if(T_Serialization.find(Serialize(X)) == T_Serialization.end()) {
                return false;
            }
        }
        return true;
    }
    std::string Serialize(chordalg::Subset set) const {
        std::stringstream ss;
        for(int x : set) {
            ss << x << " ";
        }
        return ss.str();
    }
  protected:
    chordalg::LexTrie* T_;
    std::set< std::string > T_Serialization;
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
    this->InsertAndSerialize({2, 4, 9});  
    EXPECT_EQ(new_insert_, true);
    this->InsertAndSerialize({2, 3, 5});
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieSerializedSet(), true);
}

// Test internal node set indicators
TEST_F(LexTrieTest, InternalSet) {
    this->InitTrie(10);
    this->InsertAndSerialize({2, 4});
    EXPECT_EQ(new_insert_, true);
    this->InsertAndSerialize({2});          // Internal to existing branch.
    EXPECT_EQ(new_insert_, true);
    this->InsertAndSerialize({2, 4, 9});    // Extend existing branch.
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieSerializedSet(), true);
    EXPECT_EQ(T_->size(), 3);
}

// Test root branching
TEST_F(LexTrieTest, RootBranch){
    this->InitTrie(10);
    this->InsertAndSerialize({2, 4, 9});
    EXPECT_EQ(new_insert_, true);
    this->InsertAndSerialize({1, 6, 7, 8});
    EXPECT_EQ(new_insert_, true);
    EXPECT_EQ(this->IsTrieSerializedSet(), true);
    EXPECT_EQ(T_->size(), 2);  
}

