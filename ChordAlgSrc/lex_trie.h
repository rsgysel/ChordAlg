/*
 *  lex_trie.h - A trie data structure to store subsets of {0, 1, ..., n-1}
 */

#ifndef CHORDALGSRC_LEX_TRIE_H_
#define CHORDALGSRC_LEX_TRIE_H_

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <utility>

namespace chordalg {

class LexTrie;
class LexTrieNode;

typedef std::map< size_t, LexTrieNode* > LexTrieNodeChildren;

// Finite set of natural numbers
class FiniteSet : public std::vector< size_t > {
 public:
    std::string str() const;
};  // FiniteSet

class LexTrieIterator {
 public:
    LexTrieIterator() = delete;

    explicit LexTrieIterator(const LexTrie*);
    LexTrieIterator(size_t, LexTrieNode*, const LexTrie*);
    LexTrieIterator(const LexTrieIterator&);

    LexTrieIterator operator++();
    bool operator==(const LexTrieIterator&) const;
    bool operator!=(const LexTrieIterator&) const;
    void operator=(const LexTrieIterator&);
    const FiniteSet& operator*() const;

 private:
    size_t n_;
    FiniteSet set_;  // Current set
    std::vector< LexTrieNode* > nodes_;  // Path in lex trie to current set
    std::vector< LexTrieNodeChildren::const_iterator > children_itrs_;
    bool only_empty_set_returned_;  // If T_ is only the empty set
    const LexTrie* T_;
};  // LexTrieIterator

// Each node may represent a set by its path to the root.
// A node is a k-child if the largest element in its set is k (equivalently,
// traversing an edge from any node to a k-child is "adding" k to the set
// we're inserting/checking).
class LexTrieNode {
 public:
    LexTrieNode(const LexTrieNode&) = delete;
    void operator=(const LexTrieNode&) = delete;
    LexTrieNode();
    ~LexTrieNode();

    bool HasChild(size_t) const;
    void AddChild(LexTrieNode*, size_t);
    LexTrieNode* GetChild(size_t);
    const LexTrieNode* GetChild(size_t) const;
    void CreateSet(size_t);
    void CreateSet(size_t, float);
    bool is_set() const;
    size_t set_id() const;
    float score() const;
    LexTrieNodeChildren& children();

 private:
    bool is_set_;
    size_t set_id_;
    float score_;
    LexTrieNodeChildren children_;
};  // LexTrieNode

// A tree representing a family of subsets of [0, 1, ..., n-1]
class LexTrie {
 public:
    LexTrie() = delete;
    LexTrie(const LexTrie&) = delete;
    void operator=(const LexTrie&) = delete;

    explicit LexTrie(size_t);
    ~LexTrie();

    bool Contains(const std::vector< size_t >&, float* score = nullptr) const;
    size_t Insert(const std::vector< size_t >&,
                  bool* new_set = nullptr,
                  float* score = nullptr);
    size_t PresortedInsert(const std::vector< size_t >&,
                        bool* new_set = nullptr,
                        float* score = nullptr);
    std::string str() const;
    size_t size() const;        // number of sets in family
    size_t n() const;
    LexTrieIterator begin() const;
    LexTrieIterator end() const;

 private:
    size_t n_;  // size of original set
    size_t set_count_;  // number of sets in family
    LexTrieNode* root_;
};  // LexTrie

}  // namespace chordalg

#endif  // CHORDALGSRC_LEX_TRIE_H_

