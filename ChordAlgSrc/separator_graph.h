#ifndef CHORDALGSRC_SEPARATOR_GRAPH_H_
#define CHORDALGSRC_SEPARATOR_GRAPH_H_

#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

class SeparatorGraph : public Graph {
 public:
    SeparatorGraph() = delete;
    SeparatorGraph(const SeparatorGraph&) = delete;
    void operator=(const SeparatorGraph&) = delete;

    virtual ~SeparatorGraph();

    static SeparatorGraph* New(const Graph*);

    std::string str() const;
    VertexName name(Vertex v) const;
    using Graph::HasEdge;
    bool HasEdge(Vertex u, Vertex v) const;
    const Vertices& N(Vertex v) const;

 protected:
    SeparatorGraph(const Graph*, const MinsepTrie*);
    void ComputeCrossingRelations();

    const Graph* const G_;                  // Original graph
    const MinsepVector* const M_;

    AdjacencyLists crossing_relations_;

    SeparatorComponents S_;                 // For crossing-relation computatio
};  // SeparatorGraph

}  // namespace chordalg

#endif  // CHORDALGSRC_SEPARATOR_GRAPH_H_
