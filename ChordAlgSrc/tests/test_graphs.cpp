#include "test_graphs.h"

// From Figure 1 of Berry, Pogorelcnik, and Simonet
// `An Introduction to Clique Minimal Separator Decomposition'
// 0 1 2 3 4 5 6 7 8 9 10
// a b c d e f g h i j k
// Atoms:
// abck = 0 1 2 10 (chordless 4-cycle)
// cdjk = 2 3 9 10 (clique)
// defgjk = 3 4 5 6 9 10
// hijk = 7 8 9 10 (chordless 4-cycle)
chordalg::AdjacencyLists atom_test{ {1, 10},
                                    {0, 2},
                                    {1, 3, 9, 10},
                                    {2, 4, 5, 9, 10},
                                    {3, 6},
                                    {3, 6, 9, 10},
                                    {4, 5, 9, 10},
                                    {8, 9},
                                    {7, 10},
                                    {2, 3, 5, 6, 7, 10},
                                    {0, 2, 3, 5, 6, 8, 9} };

std::vector< chordalg::AdjacencyLists > atom_test_atoms{
    { {1, 2},
      {0, 3},
      {0, 3},
      {1, 2} },
    { {1, 2, 4, 5},
      {0, 3},
      {0, 3, 4, 5},
      {1, 2, 4, 5},
      {0, 2, 3, 5},
      {0, 2, 3, 4} },
    { {1, 2, 3},
      {0, 2, 3},
      {0, 1, 3},
      {0, 1, 2} },
    { {1, 3},
      {0, 2},
      {1, 3},
      {0, 2} } };

// Vertices 0-2 and 3-5 define the vertex-bipartition of
// a bipartite graph. 6 is adjacent to 0-2, 7 is adjacent
// to 3-5 (so the entire graph is also bipartite).
chordalg::AdjacencyLists bipartite_reduction{ {3, 4, 6},
                                              {4, 5, 6},
                                              {3, 4, 5, 6},
                                              {0, 2, 7},
                                              {0, 1, 2, 7},
                                              {1, 2, 7},
                                              {0, 1, 2},
                                              {3, 4, 5} };

chordalg::AdjacencyLists bipartite_reduction_st{ {3, 4, 6},
                                                 {4, 5, 6},
                                                 {3, 4, 5, 6},
                                                 {0, 2, 7},
                                                 {0, 1, 2, 7},
                                                 {1, 2, 7},
                                                 {0, 1, 2, 7, 8},
                                                 {3, 4, 5, 6, 8},
                                                 {6, 7} };

chordalg::AdjacencyLists bipartite_reduction_x{ {3, 4, 6},
                                                {4, 5, 6},
                                                {3, 4, 5, 6},
                                                {0, 2, 7},
                                                {0, 1, 2, 7},
                                                {1, 2, 7},
                                                {0, 1, 2, 8},
                                                {3, 4, 5, 8},
                                                {6, 7} };

chordalg::AdjacencyLists clique{ {1, 2, 3, 4},
                                 {0, 2, 3, 4},
                                 {0, 1, 3, 4},
                                 {0, 1, 2, 4},
                                 {0, 1, 2, 3} };

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

chordalg::AdjacencyLists connected_components_test{ {1, 4, 5, 6, 10},
                                                     {0, 3, 4, 10},
                                                     {4, 7, 14},
                                                     {1, 4, 9, 14},
                                                     {0, 1, 2, 3},
                                                     {0, 6},
                                                     {0, 5},
                                                     {2, 8},
                                                     {7, 9},
                                                     {3, 8},
                                                     {0, 1, 11},
                                                     {10, 12, 14},
                                                     {11, 13, 14},
                                                     {12, 14},
                                                     {2, 3, 11, 12, 13} };

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

chordalg::AdjacencyLists tree{ {8},
                               {8},
                               {9},
                               {9},
                               {10},
                               {10},
                               {11},
                               {11},
                               {0, 1, 12},
                               {2, 3, 12},
                               {4, 5, 13},
                               {6, 7, 13},
                               {8, 9, 13},
                               {10, 11, 12} };

chordalg::AdjacencyLists two_cliques{ {1, 2, 3, 4},
                                      {0, 2, 3, 4},
                                      {0, 1, 3, 4},
                                      {0, 1, 2, 4},
                                      {0, 1, 2, 3},
                                      {6},
                                      {5} };

