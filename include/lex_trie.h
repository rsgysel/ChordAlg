#ifndef LEX_TRIE_H_
#define LEX_TRIE_H_

#include<algorithm>
#include<fstream>
#include<iostream>
#include<iterator>
#include<map>
#include<vector>

#include "utilities.h"

namespace chordalg {

class LexTrie;
class LexTrieNode;

typedef std::map< int, LexTrieNode* > ChildDataStructure;

class LexTrieIterator
{
    public:
        LexTrieIterator(const LexTrie* T) : n_(0), set_(), nodes_(), children_itrs_(), T_(T) { return; }
        LexTrieIterator(int n, LexTrieNode* root, const LexTrie* T);

        LexTrieIterator& operator++();
        bool operator==(const LexTrieIterator& other) const;
        bool operator!=(const LexTrieIterator& other) const { return !( *this == other ); }
        const std::vector< int >& operator*() const { return set_; }

    private:
        int n_;
        std::vector< int > set_;					// Current set
        std::vector< LexTrieNode* > nodes_;		    // Path in lex trie to current set
        std::vector< ChildDataStructure::const_iterator > children_itrs_;
        const LexTrie* T_;

        void GetNextSetBelow();
        int GetNextChild();
        int GetNextChildAfter(int k);

        DISALLOW_DEFAULT_CONSTRUCTOR(LexTrieIterator);
};

// Each node potentially represents a set by traversing a path from the root to the node.
// A node is a k-child if the largest element in its set is k (equivalently, traversing
// an edge from any node to a k-child is "adding" k to the set we're inserting/checking).
class LexTrieNode
{
    private:
        LexTrieNode(bool has_set) : has_set_(has_set), children_() { return; }
        ~LexTrieNode();

        bool has_set_;								// true if node represents a set
        ChildDataStructure children_;

        int SizeOf(int n) const;
        inline bool HasChild(int k) const {	return (children_.find(k) != children_.end()) ? true : false; }
        inline void AddChild(LexTrieNode* node,int k) { children_[k] = node; return; }
        inline LexTrieNode* operator[](int k){ return (children_.find(k) != children_.end()) ? children_[k] : NULL; }

        // Friend functions
        friend std::ostream& operator <<(std::ostream &os,const LexTrie &obj);
        friend LexTrieIterator& LexTrieIterator::operator++();

        friend class LexTrie;
        friend class LexTrieIterator;

        DISALLOW_DEFAULT_CONSTRUCTOR(LexTrieNode);
        DISALLOW_COPY_AND_ASSIGN(LexTrieNode);
};

// A tree representing a family of subsets of [0, 1, ..., n-1]
class LexTrie
{
    public:
        LexTrie(int n);
        ~LexTrie() {delete this->root_; return;}

        template< class InputIterator > bool Contains(InputIterator begin, InputIterator end);
        template< class Container, class InputIterator > const LexTrieNode* Insert(Container set, bool& new_set = *(new bool));

        int SizeOf() const ;					            // space used by LexTrie
        unsigned int Size() const { return set_count_; }	// number of sets in family

        LexTrieIterator begin() const;
        LexTrieIterator end() const { return LexTrieIterator(this); }

    protected:
        template< class InputIterator > const LexTrieNode* InsertRange(InputIterator begin, InputIterator end, bool& new_set = *(new bool));

    private:
        int n_;						// size of original set
        int set_count_;				// number of sets in family
        LexTrieNode* root_;
        int lex_trie_id_;

        DISALLOW_DEFAULT_CONSTRUCTOR(LexTrie);
        DISALLOW_COPY_AND_ASSIGN(LexTrie);
};

//--------------------//
// Lex trie interface //
//--------------------//

// Inserts in the lex trie and returns
template< class InputIterator >
const LexTrieNode* LexTrie::InsertRange(InputIterator begin, InputIterator end, bool& new_set)
{
	LexTrieNode* node = root_;

    // Traverse trie, creating nodes if they don't exist
    for(InputIterator itr = begin; itr != end; ++itr)
    {
        if(!node->HasChild(*itr)){
            LexTrieNode* newChild = new LexTrieNode(false);
            if(newChild == NULL){ throw std::bad_alloc(); }
            node->AddChild(newChild,*itr);
        }
        node = (*node)[*itr];
    }

	if(node->has_set_)
	    new_set = false;
    else
	{
		set_count_++;
        node->has_set_ = new_set = true;
	}

	return node;
}

template< class InputIterator >
bool LexTrie::Contains(InputIterator begin, InputIterator end)
{
	LexTrieNode* node = root_;

	for(InputIterator itr = begin; itr != end; ++itr){
		if(!node->HasChild(*itr))
			return false;
		node = (*node)[*itr];
	}

	return node->has_set_;
}

template< class Container, class InputIterator  >
const LexTrieNode* LexTrie::Insert(Container set, bool& new_set)
{
    std::sort(set.begin(), set.end());
    return InsertRange< InputIterator >(set.begin(),set.end(),new_set);
}


//--------------------//
// Lex trie interface //
//--------------------//

} // namespace chordalg

#endif /*LEX_TRIE_H_*/

