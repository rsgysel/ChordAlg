#include "atom_subgraphs.hpp"
#include "classic_elimination.h"
#include "file_reader.h"
#include "intersection_graph.h"
#include "lb_elimination.h"

using namespace chordalg;

void Composite_heuristic_usage( std::string program )
{
    std::cout   << "usage: " << program << " <filename> <sep_weight>"   << std::endl
    << "where ``sep_weight'' is a real number or ``inf''"   << std::endl;
    return;
}

int main( int argc, char** argv )
{
    if( argc != 3 )
        Composite_heuristic_usage( argv[0] );
    else
    {
        Weight d, s;
        if( strncmp( argv[2], "inf", 3 ) == 0 )
        {
            d = 0;
            s = 1;
        }
        else
        {
            d = 1;
            s = std::strtod( argv[2], NULL );
        }

        std::cout << d << "*Deficiency(v) + " << s << "*Separation(v)" << std::endl;

        MatrixCellIntGraphFR* graph_reader    = NewFileReader < MatrixCellIntGraphFR >  ( argv[1] );

        ColoredIntersectionGraph G( graph_reader );
        Weight total_weight = 0;

        Atoms< ColoredIntersectionGraph > A( G );
        int clique_atoms = 0, atom_id = 0, total_count = 0;
        for( ColoredIntersectionGraph* a : A )
        {
            ++atom_id;
            if( !a->IsClique() )
            {
                LBElimination eo1( *a, new RatioCriterion() );
                LBElimination eo2( *a, new WSumCriterion( d, s) );
                ClassicElimination eo3( *a, new ClassicCriterion() );
                Weight min_weight = std::min( eo1.fill_cost(), std::min(eo2.fill_cost(), eo3.fill_cost() ) );
                if( eo1.fill_cost() == min_weight )
                {
                    total_weight    +=  eo1.fill_cost();
                    total_count     +=  eo1.fill_count();
                    std::cout << "atom (LB, Ratio)" << atom_id << std::endl;
                    eo1.PrettyPrint();
                }
                else if( eo2.fill_cost() == min_weight )
                {
                    total_weight    +=  eo2.fill_cost();
                    total_count     +=  eo2.fill_count();
                    std::cout << "atom (LB, Weighted Sum)" << atom_id << std::endl;
                    eo2.PrettyPrint();
                }
                else if( eo3.fill_cost() == min_weight )
                {
                    total_weight    +=  eo3.fill_cost();
                    total_count     +=  eo3.fill_count();
                    std::cout << "atom (Classic)" << atom_id << std::endl;
                    eo3.PrettyPrint();
                }
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
    }

    return 0;
}
