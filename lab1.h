//
// Created by 冯子毅 on 4/23/25.
//

#ifndef LAB1_LAB1_H
#define LAB1_LAB1_H
#include <set>
#include <set>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <random>
#include <limits>
#include <queue>

extern std::unordered_map<std::string, std::unordered_map<std::string, int>> graph;

std::string cleanText(const std::string& input);
void showDirectedGraph(const std::unordered_map<std::string, std::unordered_map<std::string, int>>& graph);
double calPageRank(const std::string& word);
std::string queryBridgeWords(const std::string& word1, const std::string& word2);
std::string randomWalk();
std::string generateNewText(const std::string& inputText);
std::string calcShortestPath(const std::string& word1, const std::string& word2);
int initGraph(const std::string& text);
#endif //LAB1_LAB1_H
//git change test