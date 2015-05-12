#ifndef CHORDALGSRC_TREE_H_
#define CHORDALGSRC_TREE_H_

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class Tree : public Graph {
 public:
    Tree() = delete;
    Tree(const Tree&) = delete;
    void operator=(const Tree&) = delete;

    Tree(AdjacencyLists*);
    Tree(AdjacencyLists*, VertexNames);

    bool IsTree() const;
    std::string strNewick() const;
    std::string strNewick(Vertex) const;
 protected:
    std::string NewickVisit(VertexSet*, Vertex) const;
};  // Tree

}  // namespace chordalg

#endif  // CHORDALGSRC_TREE_H_