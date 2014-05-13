#include "file_reader.h"
#include "graph_test.h"
#include "mcs.h"

TEST_F(DimacsGraphTest, MCSChordalPerfectTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ZeroFill(), true);
}

TEST_F(DimacsGraphTest, MCSNonChordalPerfectTest) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ZeroFill(), false);
}

TEST_F(DimacsGraphTest, MCSChordalFillTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ComputeFill(), 0);
}

TEST_F(DimacsGraphTest, MCSNonChordalFillTest) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ComputeFill(), 21);
}
