#include "ChordAlgSrc/lex_trie.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace chordalg {

//////////////////
// c'tors & d'tors

LexTrieIterator::LexTrieIterator(const LexTrie* T) :
    n_(0),
    set_(),
    nodes_(),
    children_itrs_(),
    only_empty_set_returned_(false),
    T_(T) {
    return;
}

LexTrieIterator::LexTrieIterator(
    size_t n,
    LexTrieNode* root,
    const LexTrie* T) :
    n_(n),
    set_(),
    nodes_(),
    children_itrs_(),
    only_empty_set_returned_(false),
    T_(T) {
    set_.reserve(n_);
    nodes_.reserve(n_);
    children_itrs_.reserve(n_);
    nodes_.push_back(root);
    children_itrs_.push_back(root->children().begin());
    return;
}

LexTrieIterator::LexTrieIterator(const LexTrieIterator& other) :
    n_(other.n_),
    set_(other.set_),
    nodes_(other.nodes_),
    children_itrs_(other.children_itrs_),
    only_empty_set_returned_(other.only_empty_set_returned_),
    T_(other.T_) {
    return;
}

LexTrieNodeData::LexTrieNodeData() :
    is_set_(false),
    set_id_(0) {
    return;
}

LexTrieNode::LexTrieNode() :
    data_(new LexTrieNodeData()),
    children_() {
    return;
}

LexTrieNode::~LexTrieNode() {
    for (auto kv : children_) {
        delete kv.second;
    }
    delete data_;
    return;
}

LexTrie::LexTrie(size_t n) : n_(n), set_count_(0) {
    try {
        this->root_ = new LexTrieNode();
    } catch(const std::bad_alloc& e) {
        std::cerr << "In LexTrie.cpp LexTrie(size_t): " << e.what();
        std::cerr << std::endl;
    }
    return;
}

LexTrie::~LexTrie() {
    delete this->root_;
    return;
}

/////////////
// Finite Set

std::string FiniteSet::str() const {
    if (empty()) {
        return std::string();
    } else {
        std::ostringstream oss;
        std::copy(begin(), end() - 1, std::ostream_iterator<size_t>(oss, " "));
        oss << back();
        return oss.str();
    }
}

//////////////////
// LexTrieIterator

LexTrieIterator LexTrieIterator::operator++() {
    // if at end of trie
    if (nodes_.empty() || T_->size() == 0) {
        return LexTrieIterator(T_);
    }
    // if T_ contains only the empty set
    if (nodes_[0]->IsSet() && T_->size() == 1) {
        nodes_.pop_back();
        children_itrs_.pop_back();
        return *this;
    }
    // otherwise Iterator either points to root, leaf, or internal node
    // we only traverse up trie if Iterator points to leaf
    if (nodes_.back()->children().empty()) {
        // climb up trie until there is a new branch
        do {
            nodes_.pop_back();
            children_itrs_.pop_back();
            set_.pop_back();
        } while (!nodes_.empty() && ++children_itrs_.back() ==
                nodes_.back()->children().end());
        // while condition is also iterating children_itrs

        // if at last set, exit
        if (nodes_.empty()) {
            return LexTrieIterator(T_);
        }
    }
    // now we dive down trie to find next set, unless T_ contains
    // only the empty set
    do {
        size_t index = children_itrs_.back()->first;
        LexTrieNode* child_node = nodes_.back()->children()[index];
        children_itrs_.push_back(child_node->children().begin());
        nodes_.push_back(child_node);
        set_.push_back(index);
    } while (!nodes_.back()->IsSet());
    return *this;
}

bool LexTrieIterator::operator==(const LexTrieIterator& other) const {
    if (nodes_.empty() && other.nodes_.empty()) {
        return T_ == other.T_;
    } else if (nodes_.empty() || other.nodes_.empty()) {
        return false;
    } else {
        return nodes_.back() == other.nodes_.back();
    }
}

bool LexTrieIterator::operator!=(const LexTrieIterator& other) const {
    return !(*this == other);
}

void LexTrieIterator::operator=(const LexTrieIterator& other) {
    this->n_ = other.n_;
    this->set_ = other.set_;
    this->nodes_ = other.nodes_;
    this->children_itrs_ = other.children_itrs_;
    this->T_ = other.T_;
    return;
}

const FiniteSet& LexTrieIterator::operator*() const {
    return set_;
}

//////////////////
// LexTrieNodeData

void LexTrieNodeData::CreateSet(size_t n) {
    is_set_ = true;
    set_id_ = n;
    return;
}

bool LexTrieNodeData::is_set() const {
    return is_set_;
}

size_t LexTrieNodeData::set_id() const {
    return set_id_;
}

//////////////
// LexTrieNode

bool LexTrieNode::HasChild(size_t k) const {
    return children_.find(k) != children_.end() ? true : false;
}

void LexTrieNode::AddChild(LexTrieNode* node, size_t k) {
    children_[k] = node;
    return;
}

LexTrieNode* LexTrieNode::GetChild(size_t k) {
    return children_.find(k) != children_.end() ? children_[k] : nullptr;
}

const LexTrieNode* LexTrieNode::GetChild(size_t k) const {
    return children_.find(k) != children_.end() ? children_.at(k) : nullptr;
}

bool LexTrieNode::IsSet() const {
    return data_->is_set();
}

size_t LexTrieNode::Id() const {
    return data_->set_id();
}

void LexTrieNode::CreateSet(size_t n) {
    data_->CreateSet(n);
    return;
}

LexTrieNodeChildren& LexTrieNode::children() {
    return children_;
}

//////////
// LexTrie

bool LexTrie::Contains(const std::vector< size_t >& X) const {
    const LexTrieNode* node = root_;
    for (auto x : X) {
        if (!node->HasChild(x)) {
            return false;
        }
        node = node->GetChild(x);
    }
    return node->IsSet();
}

size_t LexTrie::Insert(const std::vector< size_t >& X, bool* new_set) {
    std::vector< size_t > SortedX(X);
    std::sort(SortedX.begin(), SortedX.end());
    return SortedInsert(SortedX, new_set);
}

size_t LexTrie::SortedInsert(const std::vector< size_t >& X, bool* new_set) {
    LexTrieNode* node = root_;
    for (auto x : X) {
        if (!node->HasChild(x)) {
            LexTrieNode* newChild = new LexTrieNode();
            if (newChild == nullptr) {
                throw std::bad_alloc();
            }
            node->AddChild(newChild, x);
        }
        node = node->GetChild(x);
    }
    if (new_set != nullptr) {
        *new_set = node->IsSet() ? false : true;
    }
    if (!node->IsSet()) {
        ++set_count_;
        node->CreateSet(set_count_);
    }
    return node->Id();
}

std::string LexTrie::str() const {
    std::ostringstream oss;
    size_t i = 0;
    for (const FiniteSet& X : *this) {
        oss << "X" << i << ": " << X.str() << std::endl;
        ++i;
    }
    return oss.str();
}

size_t LexTrie::size() const {
    return set_count_;
}

size_t LexTrie::n() const {
    return n_;
}

LexTrieIterator LexTrie::begin() const {
    if (set_count_ == 0) {
        return end();
    } else {
        LexTrieIterator itr(n_, root_, this);
        ++itr;
        return itr;
    }
}

LexTrieIterator LexTrie::end() const {
    return LexTrieIterator(this);
}

}  // namespace chordalg
