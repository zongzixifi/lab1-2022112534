//
// Created by 冯子毅 on 5/28/25.
//
#include <gtest/gtest.h>
#include "../lab1.h"


class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        initGraph("The scientist carefully analyzed the data, wrote a detailed report, and shared the report with the team, but the team requested more data, so the scientist analyzed it again.");
    }
};
TEST(QueryBridgeWordsTest, Case1) {
    EXPECT_EQ(queryBridgeWords("analyzed", "data"), "The bridge words from analyzed to data are: the.");
}

TEST(QueryBridgeWordsTest, Case2) {
    EXPECT_EQ(queryBridgeWords("the", "but"), "The bridge words from the to but are: team.");
}

TEST(QueryBridgeWordsTest, Case3) {
    EXPECT_EQ(queryBridgeWords("data", "wrote"), "No bridge words from data to wrote!");
}

TEST(QueryBridgeWordsTest, Case4) {
    EXPECT_EQ(queryBridgeWords("report", "so"), "No bridge words from report to so!");
}

TEST(QueryBridgeWordsTest, Case5) {
    EXPECT_EQ(queryBridgeWords("it", "it"), "No bridge words from it to it!");
}

TEST(QueryBridgeWordsTest, Case6) {
    EXPECT_EQ(queryBridgeWords("man", "report"), "No man or report in the graph!");
}

TEST(QueryBridgeWordsTest, Case7) {
    EXPECT_EQ(queryBridgeWords("can", "report"), "No can or report in the graph!");
}

TEST(QueryBridgeWordsTest, Case8) {
    EXPECT_EQ(queryBridgeWords("detailed", "man"), "No detailed or man in the graph!");
}

TEST(QueryBridgeWordsTest, Case9) {
    EXPECT_EQ(queryBridgeWords("and", "is"), "No and or is in the graph!");
}

TEST(QueryBridgeWordsTest, Case10) {
    EXPECT_EQ(queryBridgeWords("", "and"), "Can not be null!");
}

TEST(QueryBridgeWordsTest, Case11) {
    EXPECT_EQ(queryBridgeWords("report", ""), "Can not be null!");
}
