/*
 *  separator.h - a data structure representing a vertex separator, and
 *  its connected components and their neighborhoods
 */

#ifndef CHORDALGSRC_SEPARATOR_H_
#define CHORDALGSRC_SEPARATOR_H_

#include <vector>
#include <string>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

typedef int ConnectedComponentID;
typedef std::vector< VertexSet > FillSet;

class Block {
 public:
    Block() : C_(), NC_() {}
    explicit Block(Vertices& C) : C_(C), NC_() {}
    Block(Vertices& C, Vertices& NC) : C_(C), NC_(NC) {}
    ~Block() {}

    const Vertices& C() const {
        return C_;
    }
    const Vertices& NC() const {
        return NC_;
    }

    void addC(Vertex v) {
        C_.add(v);
    }
    void addNC(Vertex v) {
        NC_.add(v);
    }

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
    virtual ~SeparatorComponents() {}

    Vertices GetNeighborhood(Vertex, FillSet&);
    virtual void SeparateNbhd(Vertex);
    virtual void SeparateClosedNbhd(Vertex);
    virtual void Separate(const Vertices&);
    virtual void Separate(const Vertices&, FillSet&);
    Vertices ConnectedComponent(Vertex) const;

    size_t size() const {
        return size_;
    }

    ConnectedComponentID ComponentId(Vertex v) const {
        return connected_component_[v];
    }

    ConnectedComponentID kInSeparator() const {
        return -2;
    }
    ConnectedComponentID kUnsearched() const {
        return -1;
    }
    ConnectedComponentID kRemoved() const {
        return -3;
    }

    inline bool IsInSeparator(Vertex u) const {
        return connected_component_[u] == kInSeparator();
    }

    inline bool AreConnected(Vertex u, Vertex v) const {
        return IsInSeparator(u) || IsInSeparator(v) ? false :
               connected_component_[u] == connected_component_[v];
    }

    // AreSeparated is not the complement of AreConnected, because vertices in
    // S_ are neither connected or separated
    inline bool AreSeparated(Vertex u, Vertex v) const {
        return IsInSeparator(u) || IsInSeparator(v) ? false :
               connected_component_[u] != connected_component_[v];
    }

    bool IsSeparated(Vertices V) const;

    virtual std::string str() const;

 protected:
    void InitializeS(const Vertices&);
    virtual void FindComponents(FillSet&);

    const Graph* const G_;

    Vertices S_;
    std::vector< int > connected_component_;
    std::vector< int > search_queue_;
    size_t size_;  // # of connected components

    inline bool IsUnsearched(Vertex u) const {
        return connected_component_[u] == kUnsearched();
    }
};  // SeparatorComponents

class SeparatorBlocks : public SeparatorComponents {
 public:
    SeparatorBlocks() = delete;
    SeparatorBlocks(const SeparatorBlocks&) = delete;
    void operator=(const SeparatorBlocks&) = delete;

    explicit SeparatorBlocks(const Graph*);
    ~SeparatorBlocks() {}

    std::vector< Block >::const_iterator begin() {
        return blocks_.begin();
    }

    std::vector< Block >::const_iterator end() {
        return blocks_.end();
    }

    bool IsFull(ConnectedComponentID C) const {
        return S_.size() == NComponent(C).size();
    }
    const Vertices& Component(ConnectedComponentID C) const {
        return blocks_[C].C();
    }
    const Vertices& NComponent(ConnectedComponentID C) const {
        return blocks_[C].NC();
    }
    const Block& BlockOf(Vertex v) const {
        return blocks_[ComponentId(v)];
    }
    const Vertices& ComponentOf(Vertex v) const {
        return blocks_[ComponentId(v)].C();
    }
    const Vertices& NComponentOf(Vertex v) const {
        return blocks_[ComponentId(v)].NC();
    }

    size_t FullComponentCt() const;
    size_t NonFullComponentCt() const;
    std::string str() const;

 private:
    virtual void FindComponents(FillSet&);
    void FindNeighborhoods(FillSet&);

    std::vector< Block > blocks_;
    std::vector< int > last_separator_vertex_seen_;
};  // SeparatorBlocks

}  // namespace chordalg

#endif  // CHORDALGSRC_SEPARATOR_H_
