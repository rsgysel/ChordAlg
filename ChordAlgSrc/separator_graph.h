#ifndef CHORDALGSRC_SEPARATOR_GRAPH_H_
#define CHORDALGSRC_SEPARATOR_GRAPH_H_

#include <stack>
#include <string>
#include <vector>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/lex_trie.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef LexTrie                     MinsepTrie;
typedef std::stack< Vertices >      MinsepStack;
typedef std::vector< Vertices >     MinsepVector;

MinsepVector* MinsepTrieToVector(const MinsepTrie&);

class SeparatorGraph : public Graph {
 public:
    SeparatorGraph() = delete;
    SeparatorGraph(const SeparatorGraph&) = delete;
    void operator=(const SeparatorGraph&) = delete;

    virtual ~SeparatorGraph();

    static SeparatorGraph* New(const Graph*);

    std::string str() const;
    VertexName name(Vertex v) const;
    bool HasEdge(Vertex u, Vertex v) const;
    const Vertices& N(Vertex v) const {
        return crossing_relations_[v];
    }

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
