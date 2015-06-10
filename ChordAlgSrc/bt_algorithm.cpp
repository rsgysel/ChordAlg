#include "ChordAlgSrc/bt_algorithm.h"

#include <algorithm>
#include <limits>
#include <map>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/minimal_separators.h"
#include "ChordAlgSrc/potential_maxcliques.h"
#include "ChordAlgSrc/separator.h"
#include "ChordAlgSrc/vertices.h"

namespace chordalg {

namespace BTScheme {

unsigned long long GraphParameter::PMCRScore(
    const Graph& G,
    const SeparatorBlocks& S,
    const Block& B,
    const Vertices& K,
    const LexTrie& T) const {
    unsigned long long result = InitialPMCRScore(G, B, K);
    for (const Block& b : S) {
        if (B.IsSuperBlock(b)) {
            unsigned long long block_score;
            T.Contains(b.C(), &block_score);
            result = AccountForBlockScore(result, block_score);
        }
    }
    return result;
}

unsigned long long CliqueType::MSRScore(
    const Graph& G,
    const SeparatorBlocks& B,
    const LexTrie& T) const {
    (void) G;
    unsigned long long max_block_score = 0;
    for (const Block& b : B) {
        unsigned long long block_score =
            std::numeric_limits< unsigned long long >::max();
        T.Contains(b.C(), &block_score);
        max_block_score = std::max(max_block_score, block_score);
    }
    return max_block_score;
}

unsigned long long CliqueType::CliqueScore(const Graph& G) const {
    return g(G, G.V());
}

unsigned long long CliqueType::InitialPMCRScore(
    const Graph& G,
    const Block& B,
    const Vertices& K) const {
    (void) B;
    return g(G, K);
}

unsigned long long CliqueType::AccountForBlockScore(
    unsigned long long score,
    unsigned long long block_score) const {
    return std::max(score, block_score);
}

unsigned long long FillType::g(const Graph& G, const Vertices& V) const {
    unsigned long long result = 0;
    for (VertexPair uv : VertexPairs(V)) {
        if (!G.IsEdge(uv)) {
            result += g(G, uv);
        }
    }
    return result;
}

unsigned long long FillType::MSRScore(
    const Graph& G,
    const SeparatorBlocks& B,
    const LexTrie& T) const {
    unsigned long long total_score = 0;
    for (const Block& b : B) {
        if (total_score == 0) {
            total_score += g(G, b.NC());
        }
        unsigned long long block_score =
            std::numeric_limits< unsigned long long >::max();
        T.Contains(b.C(), &block_score);
        total_score += block_score;        
    }
    return total_score;
}

unsigned long long FillType::CliqueScore(const Graph& G) const {
    return g(G, Vertices());
}

unsigned long long FillType::InitialPMCRScore(
    const Graph& G,
    const Block& B,
    const Vertices& K) const {
    return g(G, K) - g(G, B.NC());
}

unsigned long long FillType::AccountForBlockScore(
    unsigned long long score,
    unsigned long long block_score) const {
    return score + block_score;
}

unsigned long long TreeWidth::g(const Graph& G, const Vertices& V) const {
    (void) G;
    return V.size() - 1;
}

unsigned long long WeightedMinimumFill::g(
    const Graph& G,
    const VertexPair& uv) const {
    return G.FillCount(uv);
}

GoodTripleBuckets* GoodTripleBuckets::New(
    const Graph* G,
    const MinsepTrie& T) {
    GoodTripleBuckets* result = new GoodTripleBuckets(G);
    // Generate full blocks
    SeparatorBlocks B(G);
    for (const FiniteSet& X : T) {
        Vertices S(X);
        B.Separate(S);
        for (const Block& b : B) {
            if (b.IsFull()) {
                // A full-block is inclusion minimal
                // iff its connected component is a clique
                result->Insert(b);
            }
        }
    }
    // Add potential maxcliques to good triples
    PMCTrie* P = PotentialMaxcliques::Generate(*G);
    SeparatorBlocks FB(G);
    for (const FiniteSet& X : *P) {
        Vertices K(X);
        result->ProcessPotentialMaxclique(K, B, FB);
    }
    delete P;
    return result;
}

GoodTripleBuckets::GoodTripleBuckets(const Graph* G) :
    G_(G),
    good_triples_(G_->order()),
    block_index_(G_->order()) {
    return;
}

// Insert a full block. Caller responsible for full-ness of block.
void GoodTripleBuckets::Insert(const Block& b) {
    unsigned long long index = good_triples_[b.size()].size();
    // b is uniquely defined by C, since it is full
    block_index_.Insert(b.C(), nullptr, &index);
    good_triples_[b.size()].push_back(std::make_pair(b, PMCVector()));
    return;
}

void GoodTripleBuckets::ProcessPotentialMaxclique(
    const Vertices& K,
    SeparatorBlocks& B,
    SeparatorBlocks& FB) {
    B.Separate(K);
    for (const Block& b : B) {
        FB.Separate(b.NC());
        ConnectedComponentID K_component_id = FB.kInSeparator();
        for (const Vertex& v : K) {
            if (!FB.IsInSeparator(v)) {
                K_component_id = FB.ComponentId(v);
            }
        }
        unsigned long long index = 0;
        block_index_.Contains(FB.Component(K_component_id), &index);
        good_triples_[FB.blocks()[K_component_id].size()][index].second.push_back(Vertices(K));
    }
    (void) K;
    return;
}

const std::vector< GoodTripleVector >& GoodTripleBuckets::good_triples() const {
    return good_triples_;
}

unsigned long long Run(const std::string& filename, const CliqueType& f_c) {
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    unsigned long long result = 0;
    for (auto a : *A) {
        result = std::max(result, Run(*a, f_c));
    }
    delete A;
    delete G;
    return result;
}

unsigned long long Run(const std::string& filename, const FillType& f_f) {
    Graph* G = Graph::New(filename);
    Atoms* A = Atoms::New(G);
    unsigned long long result = 0;
    for (auto a : *A) {
        result += Run(*a, f_f);
    }
    delete A;
    delete G;
    return result;
}

unsigned long long Run(const Graph& G, const GraphParameter& F) {
    unsigned long long result =
        std::numeric_limits< unsigned long long >::max();

    // Quick exit if clique
    if (G.IsClique()) {
        return F.CliqueScore(G);
    }

    // Generate full_blocks and potential maximal cliques
    MinsepTrie* T = MinimalSeparators::Generate(G);
    GoodTripleBuckets* GTB = GoodTripleBuckets::New(&G, *T);
    MinsepVector* IMS = MinimalSeparators::FilterInclusionMinimal(G, *T);
    LexTrie block_score(G.order());
    SeparatorBlocks B(&G);

    for (auto bucket : GTB->good_triples()) {
        for (auto good_triple : bucket) {
            const Block& b = good_triple.first;
            Vertices block_vertices;
            block_vertices.SetUnion(b.C(), b.NC());
            if (G.IsClique(block_vertices)) {
                unsigned long long score = F.g(G, block_vertices);
                block_score.Insert(b.C(), nullptr, &score);
            } else {
                const PMCVector& pmcs_containing_block = good_triple.second;
                unsigned long long score =
                    std::numeric_limits< unsigned long long >::max();
                for (const Vertices& K : pmcs_containing_block) {
                    B.Separate(Vertices(K));
                    score =
                        std::min(score,
                                 F.PMCRScore(G, B, b, K, block_score));
                }
                block_score.Insert(b.C(), nullptr, &score);
            }
        }
    }

    for (auto X : *IMS) {
        Vertices S(X);
        B.Separate(S);
        result = std::min(result, F.MSRScore(G, B, block_score));
    }

    delete IMS;
    delete GTB;
    delete T;
    return result;
}

}  // namespace BTScheme

}  // namespace chordalg

