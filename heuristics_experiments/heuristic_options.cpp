#include "heuristic_options.h"

const std::string
    preamble = " SOURCEFILE [OPTION] ...\nOptions:\n",
    file_opt = "-f, --file\n\tInput file\n",
    atoms_opt = "-a, --atoms\n\tRun heuristic on atom subgraphs\n",
    runs_opt = "-r, --runs\n\tNumber of times heuristic is run\n"
               "\t\tIf supplied with -a, heuristic is run multiple"
               "times on each atom subgraph independently",
    sep_opt = "-s, --sep\n\tSeparation weight\n"
              "\t\tIf `inf' supplied, deficiency is ignored\n";

void HeuristicOptions(int argc,
                      char* argv[],
                      std::string usage,
                      std::string *filename,
                      size_t* const runs,
                      bool* const atoms,
                      float* const deficiency_wt,
                      float* const separation_wt) {
    bool filename_set = false,
         runs_set = false,
         def_set = false, 
         sep_set = false;
    static struct option long_options[] = {
        {"a",       no_argument,       0,  0 },
        {"atoms",   no_argument,       0, 'a'},
        {"def",     required_argument, 0, 'd'},
        {"d",       required_argument, 0, 'd'},
        {"f",       required_argument, 0, 'f'},
        {"file",    required_argument, 0, 'f'},
        {"r",       required_argument, 0,  0 },
        {"runs",    required_argument, 0, 'r'},
        {"s",       required_argument, 0, 's'},
        {"sep",     required_argument, 0, 's'},
        {0,         0,                 0,  0 }
    };
    while (1) {
        char c = getopt_long(argc, argv, "ad:f:r:s:", long_options, nullptr);
        if (c == -1) {
            if(!filename_set) {
                std::cerr << argv[0] << ": filename must be provided\n";
                std::cerr << usage << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        }
        switch (c) {
            case 'a':
                if (atoms) {
                    *atoms = true;
                } else {
                    std::cerr << usage << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'd':
                if (def_set) {
                    std::cerr << argv[0] << ": multiple deficiency weights specified\n";
                    exit(EXIT_FAILURE);
                } else if (deficiency_wt) {
                    *deficiency_wt = std::stof(optarg);
                    def_set = true;
                } else {
                    std::cerr << usage << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'f':
                *filename = optarg;
                filename_set = true;
            break;
            case 'r':
                if (runs_set) {
                    std::cerr << argv[0] << ": multiple runs specified\n";
                    exit(EXIT_FAILURE); 
                } else if (runs) {
                    *runs = std::stod(optarg);
                    runs_set = true;
                } else {
                    std::cerr << usage << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
            case 's':
                if (sep_set) {
                    std::cerr << argv[0] << ": multiple separation weights specified\n";
                    exit(EXIT_FAILURE);
                } else if (deficiency_wt && separation_wt) {
                    std::string inf_opt("inf");
                    if (inf_opt.compare(optarg)) {
                        if(!def_set) {
                            *deficiency_wt = 1;
                        }
                        *separation_wt = std::stof(optarg);
                        sep_set = true;
                    } else {
                        if(def_set) {
                            std::cerr << argv[0] << ": separation weight inf and deficiency weight supplied\n";
                            exit(EXIT_FAILURE);
                        }
                        *deficiency_wt = 0;
                        *separation_wt = 1;
                        def_set = true;
                        sep_set = true; 
                    } // 
                } else {
                    std::cerr << usage << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
            case '?':
                std::cerr << usage << std::endl;
                exit(EXIT_FAILURE);
            break;
            default:
                std::cerr << "?? getopt returned character code 0" << c << "??\n";
                exit(EXIT_FAILURE);
        } 
    }  // getopts
    return;
}
