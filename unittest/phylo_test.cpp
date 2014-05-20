#include "graph_test.h"
#include "mcs.h"

TEST_F( MatrixCellIntGraphTest, ChordalPhyloTest )
{
    Read( graph_dir() + std::string( "phylo_test.m" ) );
    chordalg::EliminationOrder eo = chordalg::MCS(*G);
    EXPECT_EQ(eo.ZeroFill(), true);
}



TEST_F( MatrixCellIntGraphTest, SimplePhyloTest )
{
    Read( graph_dir() + std::string( "phylo_test.m" ) );
    chordalg::CliqueTree* ct = chordalg::MCSCliqueTree(*G);


    std::stringstream phylo_buffer;
    std::streambuf* old = std::cout.rdbuf(phylo_buffer.rdbuf());
    ct->PhyloNewickPrint(*H);
    std::cout.rdbuf( old );
    std::stringstream check_phylo;
    old = std::cout.rdbuf(check_phylo.rdbuf());
    // These strings have been carefully hand-checked, see jpgs in graphfiles
    std::cout << "((1,(4 5,(2 5,3 5)5)5)5)0;" << std::endl;
    std::cout.rdbuf( old );

    std::stringstream ct_buffer;
    old = std::cout.rdbuf(ct_buffer.rdbuf());
    ct->NewickPrint();
    std::cout.rdbuf( old );
    std::stringstream check_ct;
    old = std::cout.rdbuf(check_ct.rdbuf());
    // These strings have been carefully hand-checked, see jpgs in graphfiles
    std::cout << "(({0#1 2#1 3#1 4#1 5#0 6#1 1#0},({0#1 1#1 2#1 3#1 5#1 6#1 4#0},({0#1 1#1 3#1 4#1 5#1 6#0 2#0},{0#1 1#1 2#1 4#1 5#1 6#0 3#0}){0#1 1#1 2#1 3#1 4#1 5#1 6#0}){0#1 1#1 2#1 3#1 4#1 6#1 5#1}){1#1 2#1 3#1 4#1 5#0 6#1 0#1}){0#0 1#1 2#1 3#1 4#1 5#0 6#1};" << std::endl;
    std::cout.rdbuf( old );

    EXPECT_EQ(check_phylo.str().compare(phylo_buffer.str()) ,0);
    EXPECT_EQ(check_ct.str().compare(ct_buffer.str()) ,0);
}
