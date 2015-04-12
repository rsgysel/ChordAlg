#include "lex_trie.h"

//----------------------------//
// Constructors / Destructors //
//----------------------------//

namespace chordalg {

LexTrieIterator::LexTrieIterator(size_t n, LexTrieNode* root, const LexTrie* T) :
    n_(n),
    set_(),
    nodes_(),
    children_itrs_(),
    T_(T) {
    set_.reserve(n_);
    nodes_.reserve(n_);
    children_itrs_.reserve(n_);
    nodes_.push_back(root);
    children_itrs_.push_back(root->children_.begin());
    return;
}

LexTrie::LexTrie(size_t n) : n_(n), set_count_(0) {
    try {
        this->root_ = new LexTrieNode(false);
    } catch(const std::bad_alloc& e) {
        std::cerr << "In LexTrie.cpp LexTrie(size_t): " << e.what() << std::endl;
    }
    return;
}

LexTrieNode::~LexTrieNode() {
    for (ChildData kv : children_) {
        delete kv.second;
    }
    return;
}

//----------------------------//
// Constructors / Destructors //
//----------------------------//

//------------------//
// Member functions //
//------------------//

void LexTrie::PrettyPrint() const {
    size_t i = 0;
    for(const Subset& X : *this) {
        std::cout << "X" << i << ": ";
        std::copy(X.begin(), X.end(), std::ostream_iterator<Element>(std::cout<< " "));
        std::cout << std::endl;
        ++i;
    }
    return;
}

size_t LexTrie::SizeOf() const {
    return sizeof(*this) + root_->SizeOf(n_);
}

size_t LexTrieNode::SizeOf(size_t n) const {
    size_t size = 0;
    for (ChildData kv : children_) {
        size += kv.second->SizeOf(n);
    }
    // may need to fix this
    // size of subtrie + size of node class + size of fast_array arrays
    // + size of fast_array class
    return size + sizeof(*this);
}

//------------------//
// Member functions //
//------------------//

//--------------------//
// Iterator Functions //
//--------------------//

LexTrieIterator LexTrie::begin() const {
    if (set_count_ == 0) {
        return end();
    } else {
        LexTrieIterator itr(n_, root_, this);
        ++itr;
        return itr;
    }
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

LexTrieIterator LexTrieIterator::operator++() {
    // if at end of trie
    if (nodes_.empty() || T_->size() == 0) {
        return LexTrieIterator(T_);
    }
    // otherwise Iterator either points to root, leaf, or internal node
    // we only traverse up trie if Iterator points to leaf
    if (nodes_.back()->children_.empty()) {
        // climb up trie until there is a new branch
        do {
            nodes_.pop_back();
            children_itrs_.pop_back();
            set_.pop_back();
        } while (!nodes_.empty() && ++children_itrs_.back() ==
                nodes_.back()->children_.end());
        // while condition is also iterating children_itrs

        // if at last set, exit
        if ( nodes_.empty() ) {
            return LexTrieIterator(T_);
        }
    }
    // now we dive down trie to find next set
    do {
        size_t index = children_itrs_.back()->first;
        LexTrieNode* child_node = nodes_.back()->children_[index];
        children_itrs_.push_back(child_node->children_.begin());
        nodes_.push_back(child_node);
        set_.push_back(index);
    } while (!nodes_.back()->has_set_);
    return *this;
}

//--------------------//
// Iterator Functions //
//--------------------//

}  // namespace chordalg
