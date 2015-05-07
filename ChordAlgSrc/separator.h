/*
 *  separator.h - a data structure representing a vertex separator, and
 *  its connected components and their neighborhoods
 */

#ifndef CHORDALGSRC_SEPARATOR_H_
#define CHORDALGSRC_SEPARATOR_H_

#include <vector>
#include <string>

#include "ChordAlgSrc/fill_edges.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef int ConnectedComponentID;

class Block {
 public:
    Block();
    explicit Block(const Vertices&);
    Block(const Vertices&, const Vertices&);

    const Vertices& C() const;
    const Vertices& NC() const;
    void addC(Vertex v);
    void addNC(Vertex v);

 private:
    Vertices C_, NC_;
};  // Block

// Calculates the connected components of G - S for a graph G and vertex set S
// Intended for numerous computations on the same graph.
class SeparatorComponents {
 public:
    SeparatorComponents() = delete;
    SeparatorComponents(const SeparatorComponents&) = delete;
    void operator=(const SeparatorComponents&) = delete;

    explicit SeparatorComponents(const Graph*);
    virtual ~SeparatorComponents();

    Vertices GetNeighborhood(Vertex, const FillEdges* fill = nullptr);
    virtual void SeparateClosedNbhd(Vertex, const FillEdges* fill = nullptr);
    virtual void Separate(const Vertices&, const FillEdges* fill = nullptr);
    Vertices ConnectedComponent(Vertex) const;

    size_t size() const;
    ConnectedComponentID ComponentId(Vertex v) const;
    ConnectedComponentID kInSeparator() const;
    ConnectedComponentID kUnsearched() const;
    ConnectedComponentID kRemoved() const;
    bool IsInSeparator(Vertex u) const;
    bool AreConnected(Vertex u, Vertex v) const;
    // AreSeparated is not the complement of AreConnected, because vertices in
    // S_ are neither connected or separated
    bool AreSeparated(Vertex u, Vertex v) const;
    bool IsSeparated(Vertices V) const;
    virtual std::string str() const;

 protected:
    void InitializeS(const Vertices&);
    virtual void FindComponents(const FillEdges* fill = nullptr);
    bool IsUnsearched(Vertex u) const;

    const Graph* const G_;

    Vertices S_;
    std::vector< int > connected_component_;
    std::vector< int > search_queue_;
    size_t size_;  // # of connected components
};  // SeparatorComponents

class SeparatorBlocks : public SeparatorComponents {
 public:
    SeparatorBlocks() = delete;
    SeparatorBlocks(const SeparatorBlocks&) = delete;
    void operator=(const SeparatorBlocks&) = delete;

    explicit SeparatorBlocks(const Graph*);
    ~SeparatorBlocks();

    std::vector< Block >::const_iterator begin();
    std::vector< Block >::const_iterator end();
    bool IsFull(ConnectedComponentID C) const;
    const Vertices& Component(ConnectedComponentID C) const;
    const Vertices& NComponent(ConnectedComponentID C) const;
    const Block& BlockOf(Vertex v) const;
    const Vertices& ComponentOf(Vertex v) const;
    const Vertices& NComponentOf(Vertex v) const;

    size_t FullComponentCt() const;
    size_t NonFullComponentCt() const;
    std::string str() const;

 private:
    virtual void FindComponents(const FillEdges*);
    void FindNeighborhoods(const FillEdges*);

    std::vector< Block > blocks_;
    std::vector< int > last_separator_vertex_seen_;
};  // SeparatorBlocks

}  // namespace chordalg

#endif  // CHORDALGSRC_SEPARATOR_H_
