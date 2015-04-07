#include "test_graphs.h"

chordalg::AdjacencyLists cig_test{ {4, 9},
                                   {5, 6, 8, 9},
                                   {7},
                                   {5, 6, 8, 9},
                                   {0, 6, 8, 9},
                                   {1, 3, 6, 8, 9},
                                   {1, 3, 4, 5, 8, 9},
                                   {2, 10},
                                   {1, 3, 4, 5, 6, 9},
                                   {0, 1, 3, 4, 5, 6, 8},
                                   {7} };


chordalg::AdjacencyLists two_cliques{ {1, 2, 3, 4},
                                      {0, 2, 3, 4},
                                      {0, 1, 3, 4},
                                      {0, 1, 2, 4},
                                      {0, 1, 2, 3},
                                      {6},
                                      {5} };

