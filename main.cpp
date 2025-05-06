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
        std::cin.ignore();  // 清除换行符
        if (choice == 0) break;

        std::string word1, word2, text;
        switch (choice) {
            case 1:
                showDirectedGraph(graph);
                break;
            case 2:
                std::cout << "输入两个英文单词：";
                std::cin >> word1 >> word2;
                std::cout << queryBridgeWords(word1, word2) << "\n";
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
                    for (const auto& [target, _] : graph) {
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

std::string cleanText(const std::string& input) {
    std::string cleaned;
    for (char ch : input) {
        if (std::isalpha(ch)) {
            cleaned += std::tolower(ch);
        } else {
            cleaned += ' ';
        }
    }

    // 去除多余空格（多个空格合成一个）
    std::istringstream iss(cleaned);
    std::string word, result;
    while (iss >> word) {
        if (!result.empty()) result += " ";
        result += word;
    }

    return result;
}

void showDirectedGraph(const std::unordered_map<std::string, std::unordered_map<std::string, int>>& graph) {

    // 输出图结构
    std::cout << "\n图的邻接表（边权为共现次数）：\n";
    for (const auto& from : graph) {
        std::cout << from.first << " -> ";
        for (const auto& to : from.second) {
            std::cout << "(" << to.first << ", " << to.second << ") ";
        }
        std::cout << std::endl;
    }

    std::ofstream dotFile("output.dot");
    dotFile << "digraph G {\n";
    for (const auto& from : graph) {
        for (const auto& to : from.second) {
            dotFile << "  \"" << from.first << "\" -> \"" << to.first << "\" [label=\"" << to.second << "\"];\n";
        }
    }
    dotFile << "}\n";
    dotFile.close();
    std::cout << "DOT 文件已保存为 output.dot\n";

    int result = std::system("dot -Tpng output.dot -o graph.png");
    if (result == 0) {
        std::cout << "已生成图像文件 graph.png\n";
    } else {
        std::cout << "无法生成图像文件，请确保已安装 Graphviz 并配置 dot 命令。\n";
    }
}

// PageRank 计算函数
double calPageRank(const std::string& word) {
    const double damping = 0.85;
    const int max_iter = 100;
    const double tol = 1e-6;

    std::unordered_map<std::string, double> pr, new_pr;
    const int N = graph.size();
    if (N == 0 || graph.find(word) == graph.end()) return 0.0;
    // 使用TF-IDF进行初始化
    std::unordered_map<std::string, int> freq;
    int total_freq = 0;
    for (const auto& [from, edges] : graph) {
        for (const auto& [to, count] : edges) {
            freq[from] += count;
            total_freq += count;
        }
    }
    for (const auto& [node, _] : graph) {
        pr[node] = (total_freq > 0) ? static_cast<double>(freq[node]) / total_freq : 1.0 / N;
    }

    for (int iter = 0; iter < max_iter; ++iter) {
        double diff = 0.0;
        for (const auto& [u, _] : graph) {
            double sum = 0.0;
            for (const auto& [v, nbrs] : graph) {
                if (nbrs.find(u) != nbrs.end()) {
                    int out_deg = nbrs.size();
                    if (out_deg > 0)
                        sum += pr[v] / out_deg;
                }
            }
            new_pr[u] = (1.0 - damping) / N + damping * sum;
            diff += std::abs(new_pr[u] - pr[u]);
        }
        pr = new_pr;
        if (diff < tol) break;
    }

    return pr[word];
}

std::string queryBridgeWords(const std::string& word1, const std::string& word2) {
    std::ostringstream result;

    if (graph.find(word1) == graph.end() || graph.find(word2) == graph.end()) {
        result << "No " << word1 << " or " << word2 << " in the graph!";
        return result.str();
    }

    std::vector<std::string> bridges;
    for (const auto& mid : graph.at(word1)) {
        if (graph.find(mid.first) != graph.end() && graph.at(mid.first).count(word2)) {
            bridges.push_back(mid.first);
        }
    }

    if (bridges.empty()) {
        result << "No bridge words from " << word1 << " to " << word2 << "!";
    } else {
        result << "The bridge words from " << word1 << " to " << word2 << " are: ";
        for (size_t i = 0; i < bridges.size(); ++i) {
            result << bridges[i];
            if (i + 2 == bridges.size())
                result << ", and ";
            else if (i + 1 < bridges.size())
                result << ", ";
        }
        result << ".";
    }

    return result.str();
}

std::string randomWalk() {
    std::ostringstream output;
    if (graph.empty()) {
        return "图为空，无法进行遍历。\n";
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::string> nodes;
    for (const auto& pair : graph) nodes.push_back(pair.first);
    std::uniform_int_distribution<> startDist(0, nodes.size() - 1);
    std::string current = nodes[startDist(gen)];

    std::set<std::pair<std::string, std::string>> visitedEdges;
    std::vector<std::string> path;
    path.push_back(current);

    output << "开始随机游走，起点：" << current << "\n";

    while (graph.count(current) && !graph.at(current).empty()) {
        const auto& neighbors = graph.at(current);
        std::vector<std::string> choices;
        for (const auto& [to, _] : neighbors) {
            choices.push_back(to);
        }

        if (choices.empty()) break;

        std::uniform_int_distribution<> nextDist(0, choices.size() - 1);
        std::string next = choices[nextDist(gen)];

        if (visitedEdges.count({current, next})) {
            output << "遇到重复边：" << current << " -> " << next << "，停止。\n";
            break;
        }

        visitedEdges.insert({current, next});
        current = next;
        path.push_back(current);
    }

    std::ofstream outFile("random_walk.txt");
    for (size_t i = 0; i < path.size(); ++i) {
        output << path[i];
        outFile << path[i];
        if (i + 1 < path.size()) {
            output << " ";
            outFile << " ";
        }
    }
    output << "\n路径已写入文件 random_walk.txt\n";
    outFile.close();
    return output.str();
}

std::string generateNewText(const std::string& inputText) {
    std::string cleaned = cleanText(inputText);
    std::istringstream iss(cleaned);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }

    if (words.size() < 2) {
        return "输入文本过短，无法进行桥接。\n";
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::ostringstream output;
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        output << words[i] << " ";

        std::vector<std::string> bridges;
        if (graph.count(words[i])) {
            for (const auto& mid : graph.at(words[i])) {
                if (graph.count(mid.first) && graph.at(mid.first).count(words[i + 1])) {
                    bridges.push_back(mid.first);
                }
            }
        }

        if (!bridges.empty()) {
            std::uniform_int_distribution<> dist(0, bridges.size() - 1);
            output << bridges[dist(gen)] << " ";
        }
    }
    output << words.back();

    return output.str();
}

bool nodeExists(const std::string& word) {
    if (graph.count(word)) return true; // 作为起点
    for (const auto& [from, edges] : graph) {
        if (edges.count(word)) return true; // 作为终点
    }
    return false;
}

std::string calcShortestPath(const std::string& word1, const std::string& word2) {
    std::ostringstream result;
    if (!nodeExists(word1) || !nodeExists(word2)) {
        std::cout << "No " << word1 << " or " << word2 << " in the graph!\n";
        return "...";
    }

    std::unordered_map<std::string, int> dist;
    std::unordered_map<std::string, std::string> prev;
    const int INF = std::numeric_limits<int>::max();

    auto dijkstra = [&](const std::string& start) {
        dist.clear();
        prev.clear();
        for (const auto& pair : graph) dist[pair.first] = INF;
        dist[start] = 0;

        using P = std::pair<int, std::string>;
        std::priority_queue<P, std::vector<P>, std::greater<>> pq;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (const auto& [v, w] : graph.at(u)) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }
    };

    dijkstra(word1);
    if (dist[word2] == INF) {
        result << "从 " << word1 << " 无法到达 " << word2 << "。\n";
        return result.str();
    }

    std::vector<std::string> path;
    for (std::string at = word2; at != word1; at = prev[at]) path.push_back(at);
    path.push_back(word1);
    std::reverse(path.begin(), path.end());

    result << "最短路径（长度 " << dist[word2] << "）：\n";
    for (size_t i = 0; i < path.size(); ++i) {
        result << path[i];
        if (i + 1 < path.size()) result << " -> ";
    }

    return result.str();
}

//git change test
//git change test