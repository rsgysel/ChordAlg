/*
 *  lex_trie.h - A trie data structure to store subsets of {0, 1, ..., n-1}.
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_LEX_TRIE_H_
#define INCLUDE_LEX_TRIE_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace chordalg {

class LexTrie;
class LexTrieNode;

typedef std::pair< size_t, LexTrieNode* > ChildData;
typedef std::map< size_t, LexTrieNode* > ChildDataStructure;

// Finite set of natural numbers
class FiniteSet : public std::vector< size_t > {
 public:
    std::string str() const;
};  // FiniteSet

class LexTrieIterator {
 public:
    explicit LexTrieIterator(const LexTrie* T) : n_(0), set_(), nodes_(),
                                                 children_itrs_(), T_(T) {}
    LexTrieIterator(size_t n, LexTrieNode* root, const LexTrie* T);

    LexTrieIterator operator++();
    bool operator==(const LexTrieIterator& other) const;

    bool operator!=(const LexTrieIterator& other) const {
        return !(*this == other);
    }

    const FiniteSet& operator*() const {
        return set_;
    }

 private:
    size_t n_;
    FiniteSet set_;  // Current set
    std::vector< LexTrieNode* > nodes_;  // Path in lex trie to current set
    std::vector< ChildDataStructure::const_iterator > children_itrs_;
    const LexTrie* T_;

    void GetNextSetBelow();
    size_t GetNextChild();
    size_t GetNextChildAfter(size_t k);

    // Disable default constructor
    LexTrieIterator();
};

// Each node may represent a set by its path to the root.
// A node is a k-child if the largest element in its set is k (equivalently,
// traversing an edge from any node to a k-child is "adding" k to the set
// we're inserting/checking).
class LexTrieNode {
 private:
    explicit LexTrieNode(bool has_set) : has_set_(has_set), children_() {}
    ~LexTrieNode();

    bool has_set_;  // true if node represents a set
    ChildDataStructure children_;

    size_t SizeOf(size_t n) const;

    inline bool HasChild(size_t k) const {
        return children_.find(k) != children_.end() ? true : false;
    }

    inline void AddChild(LexTrieNode* node, size_t k) {
        children_[k] = node;
        return;
    }

    inline LexTrieNode* GetChild(size_t k) {
        return children_.find(k) != children_.end() ? children_[k] : nullptr;
    }

    inline const LexTrieNode* GetChild(size_t k) const {
        return children_.find(k) != children_.end() ? children_.at(k) : nullptr;
    }

    // Friend functions
    friend std::ostream& operator <<(std::ostream &os, const LexTrie &obj);
    friend LexTrieIterator LexTrieIterator::operator++();

    friend class LexTrie;
    friend class LexTrieIterator;

    // Disable default constructor, copy constructor, assignment
    LexTrieNode();
    LexTrieNode(const LexTrieNode&);
    void operator=(const LexTrieNode&);
};

// A tree representing a family of subsets of [0, 1, ..., n-1]
class LexTrie {
 public:
    explicit LexTrie(size_t n);
    ~LexTrie() {
        delete this->root_;
        return;
    }

    template< class Container >
    bool Contains(Container&) const;

    template< class Container >
    bool SortedContains(const Container&) const;

    template< class Container >
    const LexTrieNode* Insert(Container&, bool* new_set = nullptr);

    template< class Container >
    const LexTrieNode* SortedInsert(const Container&, bool* new_set = nullptr);

    std::string str() const;

    size_t SizeOf() const;  // space used by LexTrie
    size_t size() const {
        return set_count_;  // number of sets in family
    }
    size_t n() const {
        return n_;
    }

    LexTrieIterator begin() const;
    LexTrieIterator end() const {
        return LexTrieIterator(this);
    }

 protected:
    template< class InputIterator >
    bool ContainsRange(InputIterator, InputIterator) const;

    template< class InputIterator >
    const LexTrieNode* InsertRange(InputIterator, InputIterator,
                                   bool* new_set = nullptr);

 private:
    size_t n_;  // size of original set
    size_t set_count_;  // number of sets in family
    LexTrieNode* root_;

    // Disable default constructor, copy constructor, assignment
    LexTrie();
    LexTrie(const LexTrie&);
    void operator=(const LexTrie&);
};

//--------------------//
// Lex trie interface //
//--------------------//

// Inserts new subset into the trie
template< class InputIterator >
const LexTrieNode* LexTrie::InsertRange(InputIterator begin, InputIterator end,
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

template< class InputIterator >
bool LexTrie::ContainsRange(InputIterator begin, InputIterator end) const {
    const LexTrieNode* node = root_;
    for (InputIterator itr = begin; itr != end; ++itr) {
        if (!node->HasChild(*itr)) {
            return false;
        }
        node = node->GetChild(*itr);
    }
    return node->has_set_;
}

template< class Container >
bool LexTrie::Contains(Container& set) const {
    std::sort(set.begin(), set.end());
    return ContainsRange< typename Container::const_iterator >(
                set.begin(), set.end());
}

template< class Container >
bool LexTrie::SortedContains(const Container& set) const {
    return ContainsRange< typename Container::const_iterator >(
                set.begin(), set.end());
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


//--------------------//
// Lex trie interface //
//--------------------//

}  // namespace chordalg

#endif  // INCLUDE_LEX_TRIE_H_

