/*
 *  bt_algorithm.h - utilities to min a clique-type or fill-type
 *  function over all minimal triangulations of a graph
 *
 *  The notation and algorithms in bt_algorithm most closely
 *  resembles those from 
 *  Furuse & Yamazaki, "A revisit of the scheme for computing 
 *  treewidth and minimum fill-in".
 *  Theoret. Comput. Sci. 531(0):66-76, 2014.
 *
 *  The original approach to solving treewidth / minim fill-in using
 *  potential maximal cliques comes from the papers
 *
 *  Bouchitte & Todinca, "Treewidth and minimum fill-in:
 *  Grouping the minimal separators".
 *  SIAM J. Comput. 31:212-232, 2001.
 *
 *  Bouchitte & Todinca, "Listing all potential maximal cliques
 *  of a graph".
 *  Theoret. Comput. Sci. 276:17-32, 2002.
 *
 *  and is improved in
 *  Fomin, Kratsch, Todinca, Villanger "Exact algorithms for treewidth
 *  and minimum fill-in".
 *  SIAM J. Comput. 38(2):1058-1079, 2008.
 */

#ifndef CHORDALGSRC_BT_ALGORITHM_H_
#define CHORDALGSRC_BT_ALGORITHM_H_

#include <map>

#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/potential_maxcliques.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace BTScheme {

class GraphParameter {
 public:
    // Parameter g over a set of vertices (CliqueType) or
    // set of fill edges on saturating vertices (FillType)
    virtual unsigned long long g(const Graph&, const Vertices&) const = 0;
    virtual unsigned long long PMCRScore(
        const Graph&,
        const SeparatorBlocks&,
        const Block&,
        const Vertices&,
        const LexTrie&) const;
    virtual unsigned long long MSRScore(
        const Graph&,
        const SeparatorBlocks&,
        const LexTrie&) const = 0;
    virtual unsigned long long CliqueScore(const Graph&) const = 0;
 protected:
    virtual unsigned long long InitialPMCRScore(
        const Graph&,
        const Block&,
        const Vertices&) const = 0;
    virtual unsigned long long AccountForBlockScore(
        unsigned long long,
        unsigned long long) const = 0; 
};  // class GraphParameter

class CliqueType : public GraphParameter {
    unsigned long long MSRScore(
        const Graph&,
        const SeparatorBlocks&,
        const LexTrie&) const;
    unsigned long long CliqueScore(const Graph&) const;
 protected:
    unsigned long long InitialPMCRScore(
        const Graph&,
        const Block&,
        const Vertices&) const;
    unsigned long long AccountForBlockScore(
        unsigned long long,
        unsigned long long) const; 
};  // class CliqueType

class FillType : public GraphParameter {
 public:
    unsigned long long g(const Graph&, const Vertices&) const;
    virtual unsigned long long g(const Graph&, const VertexPair&) const = 0;
    unsigned long long MSRScore(
        const Graph&,
        const SeparatorBlocks&,
        const LexTrie&) const;
    unsigned long long CliqueScore(const Graph&) const;
 protected:
    unsigned long long InitialPMCRScore(
        const Graph&,
        const Block&,
        const Vertices&) const;
    unsigned long long AccountForBlockScore(
        unsigned long long,
        unsigned long long) const; 
};  // class FillType

class TreeWidth : public CliqueType {
 public:
    unsigned long long g(const Graph&, const Vertices&) const;
};  // class TreeWidth

class MinimumFill : public FillType {
 public:
    using FillType::g;
    unsigned long long g(const Graph&, const VertexPair&) const;
};  // class MinimumFill

class WeightedMinimumFill : public FillType {
 public:
    using FillType::g;
    unsigned long long g(const Graph&, const VertexPair&) const;
};  // class MinimumFill

// A triple (C,S,K) is a good triple if C is both a full component
// of the minimal separator S and the potential maximal clique K.
// We use pair as the block B satisfies C = B.C(), S = B.NC()
typedef std::pair< Block, PMCVector > GoodTriples;
typedef std::vector< GoodTriples > GoodTripleVector;

class GoodTripleBuckets {
 public:
    GoodTripleBuckets() = delete;
    GoodTripleBuckets(const GoodTripleBuckets&);
    void operator=(const GoodTripleBuckets&) = delete;

    GoodTripleBuckets(const Graph*);
    static GoodTripleBuckets* New(const Graph*, const MinsepTrie&);
    void Insert(const Block&);
    void ProcessPotentialMaxclique(
        const Vertices&,
        SeparatorBlocks&,
        SeparatorBlocks&);
    const std::vector< GoodTripleVector >& good_triples() const;

 private:
    const Graph* G_;
    std::vector< GoodTripleVector > good_triples_;
    LexTrie block_index_;
};  // GoodTripleBuckets

GoodTripleBuckets* GenerateGoodTriples(const Graph&, const MinsepTrie&);

unsigned long long Run(const std::string&, const CliqueType&);
unsigned long long Run(const std::string&, const FillType&);

// Runs minimization algorithm on graph G.
// WARNING: assumes G is connected.
// Either compute connected InducedSubgraphs
// or compute AtomSubgraphs first.
unsigned long long Run(const Graph&, const GraphParameter&);

}  // namespace BTScheme

}  // namespace chordalg

#endif  // CHORDALGSRC_BT_ALGORITHM_H_
