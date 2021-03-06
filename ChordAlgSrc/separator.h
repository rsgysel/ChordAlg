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
    Block(const Graph&, size_t);

    bool IsFull() const;
    bool IsSuperBlock(const Block&) const;
    const Vertices& C() const;
    const Vertices& NC() const;
    void AddC(Vertex v);
    void AddNC(Vertex v);
    void Sort();
    size_t size() const;

 private:
    Vertices C_, NC_;
    size_t separator_size_;
    std::vector< bool > in_block_;
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

    const Vertices& GetNeighborhood(Vertex, const FillEdges* fill = nullptr);
    void SeparateClosedNbhd(Vertex, const FillEdges* fill = nullptr);
    virtual void Separate(const Vertices&,
                          const FillEdges* fill = nullptr,
                          const Vertex* v = nullptr);
    Vertices ConnectedComponent(Vertex) const;

    size_t size() const;
    ConnectedComponentID ComponentId(Vertex) const;
    ConnectedComponentID kInSeparator() const;
    ConnectedComponentID kUnsearched() const;
    ConnectedComponentID kRemoved() const;
    bool IsInSeparator(Vertex) const;
    bool AreConnected(Vertex, Vertex) const;
    // AreSeparated is not the complement of AreConnected, because vertices in
    // S_ are neither connected or separated
    bool AreSeparated(Vertex, Vertex) const;
    bool IsSeparated(const Vertices&) const;
    virtual std::string str() const;

 protected:
    void InitializeSeparator(const Vertices&, const Vertex* v = nullptr);
    virtual void FindComponents(const FillEdges* fill = nullptr);
    bool IsUnsearched(Vertex) const;

    const Graph* const G_;

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

    void Separate(const Vertices&,
                  const FillEdges* fill = nullptr,
                  const Vertex* v = nullptr);
    std::vector< Block >::const_iterator begin() const;
    std::vector< Block >::const_iterator end() const;
    const std::vector < Block >& blocks() const;
    const Vertices& Component(ConnectedComponentID) const;
    const Vertices& NComponent(ConnectedComponentID) const;
    const Block& BlockOf(Vertex) const;
    const Vertices& ComponentOf(Vertex) const;
    const Vertices& NComponentOf(Vertex) const;

    size_t FullComponentCt() const;
    size_t NonFullComponentCt() const;
    bool IsInclusionMinimal() const;
    std::string str() const;

 private:
    virtual void FindComponents(const FillEdges*);
    void FindNeighborhoods(const Vertices&, const FillEdges*);

    std::vector< Block > blocks_;
    std::vector< int > last_separator_vertex_seen_;
    size_t separator_size_;
};  // SeparatorBlocks

}  // namespace chordalg

#endif  // CHORDALGSRC_SEPARATOR_H_
