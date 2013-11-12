#ifndef ELIMINATION_ORDER_H
#define ELIMINATION_ORDER_H

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <set>
#include <vector>

#include "chordalg_types.h"
#include "graph_types.h"

namespace chordalg {

#undef MAX_WEIGHT
#define MAX_WEIGHT DBL_MAX;

struct EliminationCriterion
{
    virtual Weight Calculate    ( Weight deficiency )                   = 0;
    virtual Weight Calculate    ( Weight deficiency, Weight separated ) = 0;
}; // struct EliminationCriterion

class EliminationOrder
{
    public:
        EliminationOrder( Graph& );
        virtual ~EliminationOrder();

        void    PrettyPrint() const;

        Weight                          fill_cost()     const   { return fill_cost_;    }
        int                             fill_count()    const   { return fill_count_;   }
        const std::vector< int >&       tie_count()     const   { return tie_count_;    }

    protected:
        virtual void Init();

        // Main Methods
        void                Elimination     ( );
        VertexCost          ArgMin          ( );
        virtual VertexCost  TieBreak        ( );        // base method breaks ties uniformly
        Vertices            MonotoneNbhd    ( Vertex );

        // Helpers
        void AddEdge        ( VertexPair    );
        bool IsEdge         ( VertexPair    );
        bool IsFillEdge     ( VertexPair    );
        bool IsRemoved      ( Vertex        );
        void Saturate       ( Vertices      );

        virtual void                        Eliminate   ( Vertex ) = 0;
        virtual std::pair< Weight, Cost >   WeightOf    ( Vertex ) = 0;

        Graph&                          G_;

        Vertices                        alpha_;             // alpha[i] = ith vertex eliminated
        std::vector < int           >   alpha_inverse_;     // alpha_inverse[v] = elimination # of v

        Weight                          fill_cost_;         //
        int                             fill_count_;        // # of fill edges added (monochromatic or not)
        std::vector < VertexSet     >   fill_neighbors_;
        VertexSet                       remaining_vertices_;

        std::vector < VertexCost    >   ties_;
        std::vector < int           >   tie_count_;
}; // EliminationOrder

template< class GraphType, class FileReaderType, class EliminationType, class CriterionType >
void RunAtomHeuristic( std::string filename, CriterionType* criterion = new CriterionType(), int runs = 1 )
{
    FileReaderType* graph_reader    = NewFileReader < FileReaderType >  ( filename );

    GraphType G( graph_reader );

    Weight total_weight = 0;
    Atoms< GraphType > A( G );
    int clique_atoms = 0, atom_id = 0, total_count = 0;
    for( GraphType* a : A )
    {
        ++atom_id;
        if( !a->IsClique() )
        {
            EliminationType* best_eo = new EliminationType( *a, criterion );

            for( int i = 1; i < runs; ++i )
            {
                EliminationType* eo = new EliminationType( *a, criterion );
                if( eo->fill_cost() < best_eo->fill_cost() )
                {
                    delete best_eo;
                    best_eo = eo;
                }
            }

            total_weight    +=  best_eo->fill_cost();
            total_count     +=  best_eo->fill_count();
            std::cout << "atom " << atom_id << std::endl;
            best_eo->PrettyPrint();
            delete best_eo;

        }
        else
            ++clique_atoms;
    }

    std::cout << "fill weight: "    << total_weight << std::endl;
    std::cout << "fill count: "     << total_count  << std::endl;
    std::cout << "vertices: "       << G.order()    << std::endl;
    std::cout << "edges: "          << G.size()     << std::endl;
    std::cout << "atoms: "          << A.size()     << std::endl;
    std::cout << "clique atoms: "   << clique_atoms << std::endl;

    return;
}

template< class GraphType, class FileReaderType, class EliminationType, class CriterionType >
void RunHeuristic( std::string filename, CriterionType* criterion = new CriterionType() )
{
    FileReaderType* graph_reader    = NewFileReader < FileReaderType >  ( filename );

    GraphType G( graph_reader );

    EliminationType eo( G, criterion );
    eo.PrettyPrint();

    std::cout << "fill weight: "    << eo.fill_cost()   << std::endl;
    std::cout << "fill count: "     << eo.fill_count()  << std::endl;
    std::cout << "vertices: "       << G.order()        << std::endl;
    std::cout << "edges: "          << G.size()         << std::endl;

    return;
}

} // namespace chordalg

#endif // ELIMINATION_ORDER_H
