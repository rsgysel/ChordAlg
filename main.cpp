#include <string>
#include "graph.h"

#define TESTS_ON

#ifdef TESTS_ON
    #include"gtest/gtest.h"
    #include"unittest/graph_test.h"
#endif

int main(int argc, char **argv) {

#ifdef TEST_ON
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
    std::string file_name = std::string("graphfiles/")
                        + std::string("twocliques.sal");
    chordalg::SortedAdjacencyListFR* graph_file = chordalg::NewFileReader< chordalg::SortedAdjacencyListFR >(file_name);
    chordalg::Graph* G = new chordalg::Graph(*graph_file);
    delete G;
#endif
}
