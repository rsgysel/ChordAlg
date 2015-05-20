/*
 *  MaxCompatSupertree -  constructs a supertree
 *  from an MRP matrix representing a collection of partially labeled
 *  trees.
 */

#include <getopt.h>

#include <iostream>
#include <string>

#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/heuristic_run.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/mcs.h"
#include "ChordAlgSrc/supertree.h"
#include "ChordAlgSrc/tree_representation.h"
#include "ChordAlgSrc/triangulation.h"

using namespace chordalg;

const static std::string MaxCompatSupertreeUsage =
    " [OPTION] ...\nOptions:\n"
    "-f, --file\n\tCharacter Matrix / Nexus MRP filename\n"
    "-a, --atoms\n\tRun heuristic on atom subgraphs (clique-separator decomposition)\n"
    "-r, --runs\n\tNumber of times heuristic is run on graph / atom subgraphs\n"
    "-l, --lbelim\n\tUse LBElimination (slower; obtains minimal triangulation)\n"
    "\t-s, --sepwt\n\t\tSeparation weight. Use with --lbelim; otherwise ratio criterion used\n";

void MaxCompatSupertreeOptions(int, char**, std::string, std::string*,
    EliminationMode*, EliminationCriterion*, bool*, size_t*,
    double*, double*);

int main(int argc, char* argv[]) {
    std::string filename;
    EliminationMode mode;
    EliminationCriterion criterion;
    bool atoms;
    size_t runs;
    double deficiency_wt, separation_wt;
    MaxCompatSupertreeOptions(argc, argv, MaxCompatSupertreeUsage, &filename,
        &mode, &criterion, &atoms, &runs, &deficiency_wt, &separation_wt);
    CellIntersectionGraph* G = CellIntersectionGraph::New(filename);
    HeuristicRun* R = HeuristicRun::New(
        G, criterion, mode, atoms, runs, deficiency_wt, separation_wt);
    std::cout << R->fill_edges().fill_weight()
              << " Columns / Characters Removed:\n"
              << R->fill_summary() << '\n';
    Triangulation* H = Triangulation::New(G, R->fill_edges());
    CliqueTree* CT = MCS::NewCliqueTree(*H);
    std::cout << SuperTree::NewickStr(*CT, *G) << '\n';
    delete R;
    delete G;
    delete CT;
    return EXIT_SUCCESS;
}

void MaxCompatSupertreeOptions(
    int argc,
    char* argv[],
    std::string usage,
    std::string* matrix_filename,
    EliminationMode* mode,
    EliminationCriterion* criterion,
    bool* atoms,
    size_t* runs,
    double* deficiency_wt,
    double* separation_wt) {
    bool matrix_filename_set = false,
         defwt_set = false,
         lb_elim_set = false,
         sepwt_set = false,
         runs_set = false;
    static struct option long_options[] = {
        {"a",           no_argument,       0, 'a'},
        {"atoms",       no_argument,       0, 'a'},
        {"d",           required_argument, 0, 'd'},
        {"defwt",       required_argument, 0, 'd'},
        {"f",           required_argument, 0, 'f'},
        {"file",        required_argument, 0, 'f'},
        {"l",           no_argument,       0, 'l'},
        {"lbelim",      no_argument,       0, 'l'},
        {"r",           required_argument, 0, 'r'},
        {"runs",        required_argument, 0, 'r'},
        {"s",           required_argument, 0, 's'},
        {"sepwt",       required_argument, 0, 's'},
        {0,             0,                 0,  0 }
    };
    *mode = EliminationMode::CLASSIC;
    *criterion = EliminationCriterion::DEFICIENCY;
    *atoms = false;
    *runs = 1;
    *separation_wt = *deficiency_wt = 1;
    while (1) {
        char c = getopt_long(argc, argv, "ad:f:lr:s:", long_options, nullptr);
        if (c == -1) {
            if (!matrix_filename_set) {
                std::cerr << argv[0] << " input filename must be provided\n";
                std::cerr << argv[0] << usage;
                exit(EXIT_FAILURE);
            }
            break;
        }
        switch (c) {
            case 'a':
                *atoms = true;
            break;
            case 'd':
                if (!defwt_set) {
                    *deficiency_wt = std::stod(optarg);
                    *criterion = EliminationCriterion::WSUM;
                    defwt_set = true;
                } else {
                    std::cerr << argv[0] << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'f':
                if (!matrix_filename_set) {
                    *matrix_filename = optarg;
                    matrix_filename_set = true;
                } else {
                    std::cerr << argv[0] << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'l':
                *mode = EliminationMode::LBELIMINATION;
                *criterion = EliminationCriterion::RATIO;
            break;
            case 'r':
                if (!runs_set) {
                    *runs = std::stoi(optarg);
                    runs_set = true;
                } else {
                    std::cerr << argv[0] << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            case 's':
                if (!sepwt_set) {
                    *separation_wt = std::stod(optarg);
                    *criterion = EliminationCriterion::WSUM;
                    sepwt_set = true;
                } else {
                    std::cerr << argv[0] << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            default:
                std::cerr << argv[0] << usage;
                exit(EXIT_FAILURE);
        }
    }
    return;
}

