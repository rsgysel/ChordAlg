#include "gtest/gtest.h"

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "lex_trie.h"

#define vector_set(X) (X, X + sizeof(X) / sizeof(int))

std::string SetToString(std::vector<int> set)
{
    std::stringstream ss;
    for(int x : set)
        ss << x << " ";
    return ss.str();
}

TEST(LexTrieTest,Basics) {
    int n = 10;
    chordalg::LexTrie T(n);
    EXPECT_EQ(static_cast<int>(T.Size()),0);

    int X1[] = {2,4,9};
    std::vector<int> S1 vector_set(X1);

    bool tf = false;
    T.Insert(S1,tf);    EXPECT_EQ(tf,true);     EXPECT_EQ(static_cast<int>(T.Size()),1);
    T.Insert(S1,tf);    EXPECT_EQ(tf,false);    EXPECT_EQ(static_cast<int>(T.Size()),1);

    return;
}

TEST(LexTrieTest,Iterator) {
    int n = 10;
    chordalg::LexTrie T(n);
    EXPECT_EQ(static_cast<int>(T.Size()),0);

    // X1 and X2 test internal branching. X3 tests internal set representation. X4 tests root branching.
    int X1[] = {2,4,9}, X2[] = {2,3,5}, X3[] = {2,4}, X4[] = {1,6,7,8};
    std::vector<int> S1 vector_set(X1), S2 vector_set(X2), S3 vector_set(X3), S4 vector_set(X4);

    // Check entries with map and SetToString
    std::map< std::string, bool > T_map;
    T_map[SetToString(S1)] = T_map[SetToString(S2)] = T_map[SetToString(S3)] = T_map[SetToString(S4)] = true;

    // Insert sets one at a time, and check that iterator works at each step
    bool tf = false;
    T.Insert(S1,tf);     EXPECT_EQ(tf,true);     EXPECT_EQ(static_cast<int>(T.Size()),1);
    for(const std::vector<int>& set : T)
        EXPECT_EQ(T_map.at(SetToString(set)),true);

    T.Insert(S2,tf);     EXPECT_EQ(tf,true);     EXPECT_EQ(static_cast<int>(T.Size()),2);
    for(const std::vector<int>& set : T)
        EXPECT_EQ(T_map.at(SetToString(set)),true);

    T.Insert(S3,tf);     EXPECT_EQ(tf,true);     EXPECT_EQ(static_cast<int>(T.Size()),3);
    for(const std::vector<int>& set : T)
        EXPECT_EQ(T_map.at(SetToString(set)),true);

    T.Insert(S4,tf);     EXPECT_EQ(tf,true);     EXPECT_EQ(static_cast<int>(T.Size()),4);
    for(const std::vector<int>& set : T)
        EXPECT_EQ(T_map.at(SetToString(set)),true);

    // If we're here, T is a subset of T_map. Now check equality
    EXPECT_EQ(T.Size(),T_map.size());

    return;
}

