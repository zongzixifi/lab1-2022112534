#include "lab1.h"

int main() {
    std::string filename = "Easy Test.txt";

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件 " << filename << "\n";
        return 1;
    }

    std::string line, fullText;
    while (std::getline(file, line)) {
        fullText += line + " ";
    }
    file.close();

    std::string cleaned = cleanText(fullText);
    std::istringstream iss(cleaned);
    std::string prev, curr;
    if (iss >> prev) {
        while (iss >> curr) {
            std::cout << "加入边：" << prev << " -> " << curr << std::endl;
            graph[prev][curr]++;
            prev = curr;
        }
        // 添加虚拟终结节点，确保句尾词也纳入图中
        const std::string END_NODE = "__END__";
        graph[prev][END_NODE]++;
        std::cout << "加入边：" << prev << " -> " << END_NODE << std::endl;
    }

    while (true) {
        std::cout << "\n请选择功能：\n"
                  << "1. 展示有向图\n"
                  << "2. 查询桥接词\n"
                  << "3. 生成新文本（插入桥接词）\n"
                  << "4. 计算最短路径\n"
                  << "5. 查询单词PR值\n"
                  << "6. 随机游走\n"
                  << "0. 退出\n选择：";
        int choice;
        std::cin >> choice;
        std::cin.ignore();// 清除换行符
        if (choice == 0) break;

        std::string word1, word2, text;
        switch (choice) {
            case 1:
                showDirectedGraph(graph);
                break;
            case 2:
                std::cout << "输入两个英文单词：";
                std::cin >> word1 >> word2;
                if (word1.empty() || word2.empty()) {
                    std::cout << "输入不能为空！" << std::endl;
                } else {
                    std::cout << queryBridgeWords(word1, word2) << "\n";
                }
                break;
            case 3:
                std::cout << "请输入新文本：";
                std::getline(std::cin, text);
                std::cout << generateNewText(text) << "\n";
                break;
            case 4: {
                std::string line;
                std::cout << "输入一个或两个英文单词（用空格分隔）: ";
                std::getline(std::cin, line);
                std::istringstream iss(line);
                std::vector<std::string> tokens;
                std::string token;
                while (iss >> token) tokens.push_back(token);

                if (tokens.size() == 1) {
                    const std::string& source = tokens[0];
                    for (const auto& [target, _]: graph) {
                        if (target != source)
                            std::cout << calcShortestPath(source, target) << "\n";
                    }
                } else if (tokens.size() == 2) {
                    std::cout << calcShortestPath(tokens[0], tokens[1]) << "\n";
                } else {
                    std::cout << "请输入1个或2个英文单词。\n";
                }
                break;
            }
            case 5:
                std::cout << "输入单词：";
                std::cin >> word1;
                std::cout << "PageRank: " << calPageRank(word1) << "\n";
                break;
            case 6:
                std::cout << randomWalk() << "\n";
                break;
            default:
                std::cout << "无效输入，请重新选择。\n";
        }
    }

    return 0;
}

//git change test
//git change test