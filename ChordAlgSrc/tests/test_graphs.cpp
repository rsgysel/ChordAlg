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

chordalg::AdjacencyLists dimacs_test{ {1, 3, 6, 8},
                                      {0, 2, 5, 7},
                                      {1, 4, 6, 9},
                                      {0, 4, 5, 9},
                                      {2, 3, 7, 8},
                                      {1, 3, 10},
                                      {0, 2, 10},
                                      {1, 4, 10},
                                      {0, 4, 10},
                                      {2, 3, 10},
                                      {5, 6, 7, 8, 9} };

chordalg::AdjacencyLists nexus_test{ {1, 2},
                                     {0, 3},
                                     {0},
                                     {1} };

chordalg::AdjacencyLists two_cliques{ {1, 2, 3, 4},
                                      {0, 2, 3, 4},
                                      {0, 1, 3, 4},
                                      {0, 1, 2, 4},
                                      {0, 1, 2, 3},
                                      {6},
                                      {5} };

