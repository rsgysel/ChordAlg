/*
 *  vertices.h - iterators, pairs, and typedefs
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_VERTICES_H_
#define INCLUDE_VERTICES_H_

#include <algorithm>
#include <initializer_list>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <utility>

namespace chordalg {

// Forward declarations
class                                   Graph;
class                                   Vertices;

// Types

typedef size_t                          Vertex;
typedef std::pair< Vertex, Vertex >     VertexPair;

typedef std::list   < Vertex >          VertexList;
typedef std::set    < Vertex >          VertexSet;
typedef std::vector < Vertex >          VertexVector;

typedef VertexList::iterator            VLIterator;
typedef VertexList::const_iterator      VLConstItr;
typedef VertexSet::iterator             VSIterator;
typedef VertexSet::const_iterator       VSConstItr;
typedef VertexVector::iterator          VVIterator;
typedef VertexVector::const_iterator    VVConstItr;

typedef Vertices                        Nbhd;
typedef std::vector< Nbhd >             AdjacencyLists;

typedef std::string                     VertexName;
typedef std::vector< VertexName >       VertexNames;

typedef size_t                          Color;
typedef std::list< Color >              Multicolor;

// elimination ordering typedefs
typedef double                          Weight;         // weight of fill edge
typedef std::pair< Vertex, Weight >     VertexWeight;

class Vertices {
 public:
    typedef VertexVector::const_iterator const_iterator;

    Vertices() : V_(new VertexVector()) {}

    explicit Vertices(size_t n) : V_(new VertexVector(n, 0)) {}
    explicit Vertices(VertexList V) : V_(new VertexVector(V.begin(), V.end())) {}
    explicit Vertices(VertexSet V) : V_(new VertexVector(V.begin(), V.end())) {}
    explicit Vertices(VertexVector V) : V_(new VertexVector(V)) {}

    Vertices(std::initializer_list<Vertex> V) : V_(new VertexVector(V)) {}
    Vertices(const Vertices& other) : V_(new VertexVector(*(other.V_))) {}

    ~Vertices() {
        delete V_;
        return;
    }

    Vertices& operator=(Vertices other) {
        std::swap(V_, other.V_);
        return *this;
    }

    void add(Vertex v) {
        V_->push_back(v);
    }
    bool empty() const {
        return V_->empty();
    }
    Vertex& operator[](size_t i) {
        return (*V_)[i];
    }
    const Vertex& operator[](size_t i) const {
        return (*V_)[i];
    }
    void clear() {
        V_->clear();
    }
    void reserve(size_t n) {
        V_->reserve(n);
    }
    size_t size() const {
        return V_->size();
    }
    void sort() {
        std::sort(begin(), end());
    }

    void merge(Vertices U, Vertices W);

    VertexVector::iterator begin() {
        return V_->begin();
    }
    VertexVector::const_iterator begin() const {
        return V_->begin();
    }
    VertexVector::iterator end() {
        return V_->end();
    }
    VertexVector::const_iterator end() const {
        return V_->end();
    }

 private:
    VertexVector* V_;
};  // Vertices

class GraphVertexIterator {
 public:
    explicit GraphVertexIterator(const Graph* G) : G_(G), v_(0) {}
    explicit GraphVertexIterator(const Graph* G, Vertex v) : G_(G), v_(v) {}

    void operator++() {
        ++v_;
    }
    Vertex operator*() const {
        return v_;
    }
    bool operator!=(const GraphVertexIterator& other) const {
        return !(*this == other);
    }
    bool operator==(const GraphVertexIterator& other) const {
        return (G_ == other.G_) && (v_ == other.v_);
    }

 private:
    const Graph* const G_;
    Vertex v_;
};  // GraphVertexIterator

class GraphVertices : public Vertices {
 public:
    explicit GraphVertices(const Graph* G, size_t order) : Vertices(), G_(G), order_(order) {}

    GraphVertexIterator begin() const {
        return GraphVertexIterator(G_);
    }
    GraphVertexIterator end() const {
        return GraphVertexIterator(G_, order_);
    }

 private:
    const Graph* const G_;
    size_t order_;
};  // GraphVertices

class VertexPairs;

class VertexPairsIterator {
 public:
    explicit VertexPairsIterator(const Vertices*, Vertex, Vertex);

    void operator++();
    bool operator==(const VertexPairsIterator& other) const {
        return (V_ == other.V_) && (v1_ == other.v1_) && (v2_ == other.v2_);
    }
    bool operator!=(const VertexPairsIterator& other) const {
        return !( *this == other );
    }
    VertexPair operator*() {
        return VertexPair((*V_)[v1_], (*V_)[v2_]);
    }

 private:
    const Vertices* const V_;
    Vertex v1_, v2_;
};  // VertexPairsIterator

class VertexPairs {
 public:
    explicit VertexPairs(Vertices V) : V_(V), begin_(0), end_(V_.size()) {}

    VertexPairsIterator begin() const {
        return VertexPairsIterator(&V_, begin_, end_);
    }
    VertexPairsIterator end() const {
        return VertexPairsIterator(&V_, end_, end_);
    }

 private:
    const Vertices V_;
    size_t begin_, end_;
};  // VertexPairs

}  // namespace chordalg

#endif  // INCLUDE_VERTICES_H_
