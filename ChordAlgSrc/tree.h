/*
 * tree.h - graph-subclass representing a tree.
 *
 * The user is responsible for validating that a Tree
 * object is a tree (use IsTree).
 */

#ifndef CHORDALGSRC_TREE_H_
#define CHORDALGSRC_TREE_H_

#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class Tree : public Graph {
 public:
    Tree() = delete;
    Tree(const Tree&) = delete;
    void operator=(const Tree&) = delete;

    explicit Tree(AdjacencyLists*);
    explicit Tree(AdjacencyLists*, VertexNames);

    bool IsTree() const;
    std::string strNewick() const;
    std::string strNewick(Vertex) const;
 protected:
    std::string NewickVisit(Vertex, VertexSet*) const;
};  // Tree

}  // namespace chordalg

#endif  // CHORDALGSRC_TREE_H_
