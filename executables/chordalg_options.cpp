#include "chordalg_options.h"

const std::string
    preamble = " [OPTION] ...\nOptions:\n",
    graph_opt = "-g, --graph\n\tGraph filename\n",
    eo_opt = "-e, --eo\n\tElimination Order filename\n",
    matrix_opt = "-m, --matrix\n\tCharacter Matrix / Nexus MRP filename\n";

void ChordAlgOptions(int argc,
                     char* argv[],
                     std::string *graph_filename,
                     std::string *eo_filename,
                     FileMode mode) {
    std::string usage = argv[0] + preamble;
    std::string graph_type_opt;
    if (graph_filename) {
        if (mode == FileMode::CHARACTERMATRIX) {
            usage += matrix_opt;
        } else if (mode == FileMode::GRAPH) {
            usage += graph_opt;
        }
    }
    if (eo_filename) {
        usage += eo_opt;
    }
    bool eo_filename_set = false,
         graph_filename_set = false;
    static struct option long_options[] = {
        {"e",       required_argument, 0, 'e'},
        {"eo",      required_argument, 0, 'e'},
        {"g",       required_argument, 0, 'g'},
        {"graph",   required_argument, 0, 'g'},
        {"m",       required_argument, 0, 'm'},
        {"matrix",  required_argument, 0, 'm'},
        {0,         0,                 0,  0 }
    };
    while (1) {
        char c = getopt_long(argc, argv, "e:g:m:", long_options, nullptr);
        if (c == -1) {
            if(graph_filename && !graph_filename_set) {
                std::cerr << argv[0] << graph_type_opt << " must be provided\n";
                std::cerr << usage << std::endl;
                exit(EXIT_FAILURE);
            } else if (eo_filename && !eo_filename_set) {
                std::cerr << argv[0] << eo_opt << " must be provided\n";
                std::cerr << usage << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        }
        switch (c) {
            case 'e':
                if (eo_filename && !eo_filename_set) {
                    *eo_filename = optarg;
                    eo_filename_set = true;
                } else {
                    std::cerr << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'g':
                if (!graph_filename_set && mode == FileMode::GRAPH) {
                    *graph_filename = optarg;
                    graph_filename_set = true;
                } else {
                    std::cerr << usage;
                    exit(EXIT_FAILURE);
                }
            break;
            case 'm':
                if (!graph_filename_set && mode == FileMode::CHARACTERMATRIX) {
                    *graph_filename = optarg;
                    graph_filename_set = true;
                } else {
                    std::cerr << usage;
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
