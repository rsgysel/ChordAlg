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
    const LexTrie* T_;
};  // LexTrieIterator

// Each node may represent a set by its path to the root.
// A node is a k-child if the largest element in its set is k (equivalently,
// traversing an edge from any node to a k-child is "adding" k to the set
// we're inserting/checking).
class LexTrieNode {
 public:
    LexTrieNode() = delete;
    LexTrieNode(const LexTrieNode&) = delete;
    void operator=(const LexTrieNode&) = delete;

 private:
    explicit LexTrieNode(bool);
    ~LexTrieNode();

    bool has_set_;  // true if node represents a set
    LexTrieNodeChildren children_;

    bool HasChild(size_t) const;
    void AddChild(LexTrieNode*, size_t);
    LexTrieNode* GetChild(size_t);
    const LexTrieNode* GetChild(size_t) const;

    friend LexTrieIterator LexTrieIterator::operator++();
    friend class LexTrie;
    friend class LexTrieIterator;
};  // LexTrieNode

// A tree representing a family of subsets of [0, 1, ..., n-1]
class LexTrie {
 public:
    LexTrie() = delete;
    LexTrie(const LexTrie&) = delete;
    void operator=(const LexTrie&) = delete;

    explicit LexTrie(size_t);
    ~LexTrie();

    template< class Container >
    const LexTrieNode* Insert(Container&, bool* new_set = nullptr);
    template< class Container >
    const LexTrieNode* SortedInsert(const Container&, bool* new_set = nullptr);
    std::string str() const;
    size_t size() const;        // number of sets in family
    size_t n() const;
    LexTrieIterator begin() const;
    LexTrieIterator end() const;

 protected:
    template< class InputIterator >
    const LexTrieNode* InsertRange(InputIterator, InputIterator,
                                   bool* new_set = nullptr);

 private:
    size_t n_;  // size of original set
    size_t set_count_;  // number of sets in family
    LexTrieNode* root_;
};  // LexTrie

///////////
// Generics

template< class InputIterator >
const LexTrieNode* LexTrie::InsertRange(
    InputIterator begin,
    InputIterator end,
    bool* new_set) {
    LexTrieNode* node = root_;

    // Traverse trie, creating nodes if they don't exist
    for (InputIterator itr = begin; itr != end; ++itr) {
        if (!node->HasChild(*itr)) {
            LexTrieNode* newChild = new LexTrieNode(false);
            if (newChild == nullptr) {
                throw std::bad_alloc();
            }
            node->AddChild(newChild, *itr);
        }
        node = node->GetChild(*itr);
    }
    if (new_set != nullptr && node->has_set_) {
        *new_set = false;
    } else if (new_set != nullptr) {
        *new_set = true;
    }
    if (!node->has_set_) {
        set_count_++;
        node->has_set_ = true;
    }
    return node;
}

template< class Container >
const LexTrieNode* LexTrie::Insert(Container& set, bool* new_set) {
    std::sort(set.begin(), set.end());
    return InsertRange< typename Container::const_iterator >(
                set.begin(), set.end(), new_set);
}

template< class Container >
const LexTrieNode* LexTrie::SortedInsert(const Container& set,
                                         bool* new_set) {
    return InsertRange< typename Container::const_iterator >(
                set.begin(), set.end(), new_set);
}

}  // namespace chordalg

#endif  // CHORDALGSRC_LEX_TRIE_H_

