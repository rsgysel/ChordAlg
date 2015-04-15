#ifndef SEPARATOR_GRAPH_H_
#define SEPARATOR_GRAPH_H_

#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "graph.h"
#include "lex_trie.h"
#include "separator.h"
#include "vertices.h"

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

    SeparatorGraph(const Graph*, const MinsepVector*);
    SeparatorGraph(const Graph*, const MinsepTrie*);
    virtual ~SeparatorGraph();

    void Init();

    std::string str() const;
    VertexName name(Vertex v) const;
    bool HasEdge(Vertex u, Vertex v) const;
    const Vertices& N(Vertex v) const {
        return crossing_relations_.operator[](v);
    }

 protected:
    const Graph* const G_;                  // Original graph
    const MinsepVector* const M_;

    AdjacencyLists crossing_relations_;

    SeparatorComponents S_;                 // For crossing-relation computatio
};  // SeparatorGraph

}  // namespace chordalg

#endif  // SEPARATOR_GRAPH_H_
