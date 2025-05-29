//
// Created by 冯子毅 on 5/29/25.
//

#include <gtest/gtest.h>
#include "../lab1.h"

// 1 图为空
TEST(RandomWalkTest, EmptyGraph) {
    graph.clear();
    std::string output = randomWalk();
    EXPECT_EQ(output, "图为空，无法进行遍历。\n");
}

// 2 7 8 只有一个点
TEST(RandomWalkTest, SingleNodeNoEdge) {
    graph.clear();
    graph["A"] = {};
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("起点：A") != std::string::npos);
    EXPECT_TRUE(result.find("A\n路径已写入文件") != std::string::npos);
}

// 3 一个有环节点
TEST(RandomWalkTest, NodeSelfLoop) {
    graph.clear();
    graph["A"]["A"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("A A") != std::string::npos);
    EXPECT_TRUE(result.find("路径已写入文件") != std::string::npos);
}

// 4 两个节点
TEST(RandomWalkTest, SimpleGraphSingleEdge) {
    graph.clear();
    graph["A"]["B"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("开始随机游走，起点：") != std::string::npos);
    EXPECT_TRUE(result.find("A") != std::string::npos);
    EXPECT_TRUE(result.find("B") != std::string::npos || result.find("停止") != std::string::npos);
}

// 5 9 A-B-C
TEST(RandomWalkTest, LinearPathWithFileOutput) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["B"]["C"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("A") != std::string::npos);
    EXPECT_TRUE(result.find("C") != std::string::npos);
}

// 6 10 两节点成环
TEST(RandomWalkTest, RepeatedEdgeStopsWalk) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["B"]["A"] = 1; // 环
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("遇到重复边") != std::string::npos || result.find("停止") != std::string::npos);
}

// 7 A-B-C-D
TEST(RandomWalkTest, MultipleEdgesNoRepetition) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["B"]["C"] = 1;
    graph["C"]["D"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("路径已写入文件") != std::string::npos);
}


// 8 B没有出边
TEST(RandomWalkTest, EmptyChoicesTriggersBreak) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["B"] = {};
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("路径已写入文件") != std::string::npos);
    EXPECT_TRUE(result.find("B") != std::string::npos);
}

// 9
TEST(RandomWalkTest, NodeWithMultipleEdges) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["A"]["C"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("A") != std::string::npos);
    EXPECT_TRUE(result.find("B") != std::string::npos || result.find("C") != std::string::npos);
}
// 10
TEST(RandomWalkTest, LongPathTerminates) {
    graph.clear();
    graph["A"]["B"] = 1;
    graph["B"]["C"] = 1;
    graph["C"]["D"] = 1;
    graph["D"]["E"] = 1;
    graph["E"]["F"] = 1;
    std::string result = randomWalk();
    EXPECT_TRUE(result.find("路径已写入文件") != std::string::npos);
    EXPECT_TRUE(result.find("F") != std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
