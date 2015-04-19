#include <getopt.h>

#include <iostream>
#include <string>

extern const std::string preamble, file_opt, atoms_opt, runs_opt, def_opt, sep_opt;

void HeuristicOptions(int argc,
                      char* argv[],
                      std::string usage,
                      std::string* filename,
                      size_t* const runs=nullptr,
                      bool* const atoms=nullptr,
                      float* const deficiency_wt=nullptr,
                      float* const separation_wt=nullptr);
