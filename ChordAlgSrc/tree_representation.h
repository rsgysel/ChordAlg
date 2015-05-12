/*
 *  tree_representation.h - a data structure for tree representations
 *  and clique trees
 */

#ifndef CHORDALGSRC_TREE_REPRESENTATION_H_
#define CHORDALGSRC_TREE_REPRESENTATION_H_

#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/tree.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef std::vector< Vertices > CliqueMap;

class TreeRepresentation {
 public:
    explicit TreeRepresentation(
        const Graph*,
        AdjacencyLists*,
        CliqueMap*);
    virtual ~TreeRepresentation();

    const Graph& G() const;
    const Tree& T() const;

 protected:
    VertexNames NamesFromCliqueMap(const Graph* G, const CliqueMap* K) const;

    const Graph* G_;    // represented chordal graph
    const Tree* T_;     // host tree
    const CliqueMap* K_;  // maps nodes of T_ to cliques of G_
};  // TreeRepresentation

class CliqueTree : public TreeRepresentation {
 public:
    explicit CliqueTree(
        const Graph*,
        AdjacencyLists*,
        CliqueMap*);
    ~CliqueTree();
};  // CliqueTree

}  // namespace chordalg

#endif  // CHORDALGSRC_TREE_REPRESENTATION_H_
