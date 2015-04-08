#include "gtest/gtest.h"

#include "atoms_test_framework.h"
#include "test_graphs.h"

TEST_F(AtomsTest, AtomCount1) {
    Read(atom_test);
    EXPECT_EQ(A_->size(), 4);
}

TEST_F(AtomsTest, AtomCount2) {
    Read(two_cliques);
    EXPECT_EQ(A_->size(), 2);
}
