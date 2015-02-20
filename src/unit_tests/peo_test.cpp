#include "graph_test.h"
#include "mcs.h"

// Unit tests for MCS and elimination order

TEST_F(DimacsGraphTest, MCSChordalPerfectTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ZeroFill(), true);
    return;
}

TEST_F(DimacsGraphTest, MCSNonChordalPerfectTest) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ZeroFill(), false);
    return;
}

TEST_F(DimacsGraphTest, MCSChordalFillTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ComputeFill(), 0);
    return;
}

TEST_F(DimacsGraphTest, MCSNonChordalFillTest) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ComputeFill(), 21);
    return;
}

// MCS peo is 4 7 3 6 5 2 1
// Neighborhood of 2: 1 3 5 6
// Neighborhood of 3: 2 4 6 7
TEST_F(DimacsGraphTest, LNbhdTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.LNbhd(G->vertex("2")).size(), 3);
    EXPECT_EQ(eo.LNbhd(G->vertex("3")).size(), 2);
    return;
}

TEST_F(DimacsGraphTest, RNbhdTest) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.RNbhd(G->vertex("2")).size(), 1);
    EXPECT_EQ(eo.RNbhd(G->vertex("3")).size(), 2);
    return;
}

TEST_F(DimacsGraphTest, VertexPositionSanity) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo(*G);
for(chordalg::Vertex v : *G)
        EXPECT_EQ(eo.VertexAt(eo.PositionOf(v)), v);
    chordalg::EliminationOrder pi = chordalg::MCS(*G);
for(chordalg::Vertex v : *G)
        EXPECT_EQ(pi.VertexAt(pi.PositionOf(v)), v);
    return;
}

TEST_F(DimacsGraphTest, SetOrderSanity) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo(*G);
    chordalg::Vertex order[] = {2, 5, 3, 4, 1, 0, 6};
    chordalg::VertexVector V(order, order + sizeof(order) / sizeof(chordalg::Vertex));
    eo.SetOrder(V);
    for(int i = 0; i < eo.size(); ++i) {
        EXPECT_EQ(order[i], eo.VertexAt(i));
        EXPECT_EQ(i, eo.PositionOf(order[i]));
    }
    return;
}

TEST_F(DimacsGraphTest, MCSCliqueTree) {
    Read(graph_dir() + std::string("fiveleaftree.dimacs"));
    chordalg::CliqueTree* ct = chordalg::MCSCliqueTree(*G);
    EXPECT_EQ(ct->T().size(), 6);
    EXPECT_EQ(ct->T().order(), 7);
    delete ct;
    return;
}

TEST_F(DimacsGraphTest, ChordalIsomorphism) {
    Read(graph_dir() + std::string("chordal1.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    eo.ComputeFill();
    chordalg::Supergraph H(*G, eo.TriangNbhds());
    EXPECT_EQ(G->IsIsomorphic(H), true);
    return;
}

TEST_F(DimacsGraphTest, Triangulation) {
    Read(graph_dir() + std::string("myciel3.dimacs"));
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    eo.ComputeFill();
    chordalg::Supergraph H(*G, eo.TriangNbhds());
    EXPECT_EQ(H.size()-21,G->size());
    return;
}
