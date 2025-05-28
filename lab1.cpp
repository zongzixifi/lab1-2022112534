//
// Created by 冯子毅 on 5/28/25.
//
#include "lab1.h"

std::unordered_map<std::string, std::unordered_map<std::string, int>> graph;

int initGraph(const std::string& text){

    std::string cleaned = cleanText(text);
    std::istringstream iss(cleaned);
    std::string prev, curr;
    if (iss >> prev) {
        while (iss >> curr) {
            graph[prev][curr]++;
            prev = curr;
        }
        // 添加虚拟终结节点，确保句尾词也纳入图中
        const std::string END_NODE = "__END__";
        graph[prev][END_NODE]++;
    }
    return 0;
}

std::string cleanText(const std::string& input) {
    std::string cleaned;
    for (char ch: input) {
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
    for (const auto& from: graph) {
        std::cout << from.first << " -> ";
        for (const auto& to: from.second) {
            std::cout << "(" << to.first << ", " << to.second << ") ";
        }
        std::cout << std::endl;
    }

    std::ofstream dotFile("output.dot");
    dotFile << "digraph G {\n";
    for (const auto& from: graph) {
        for (const auto& to: from.second) {
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
    for (const auto& [from, edges]: graph) {
        for (const auto& [to, count]: edges) {
            freq[from] += count;
            total_freq += count;
        }
    }
    for (const auto& [node, _]: graph) {
        pr[node] = (total_freq > 0) ? static_cast<double>(freq[node]) / total_freq : 1.0 / N;
    }

    for (int iter = 0; iter < max_iter; ++iter) {
        double diff = 0.0;
        // 处理 dangling nodes（出度为0的节点）
        double dangling_sum = 0.0;
        for (const auto& [node, nbrs]: graph) {
            if (nbrs.empty()) {
                dangling_sum += pr[node];
            }
        }
        for (const auto& [u, _]: graph) {
            double sum = 0.0;
            for (const auto& [v, nbrs]: graph) {
                if (nbrs.find(u) != nbrs.end()) {
                    int out_deg = nbrs.size();
                    if (out_deg > 0) {
                        sum += pr[v] / out_deg;
                    }
                }
            }
            new_pr[u] = (1.0 - damping) / N + damping * (sum + dangling_sum / N);
            diff += std::abs(new_pr[u] - pr[u]);
        }
        pr = new_pr;
        if (diff < tol) break;
    }

    return pr[word];
}

std::string queryBridgeWords(const std::string& word1, const std::string& word2) {
    initGraph("The scientist carefully analyzed the data, wrote a detailed report, and shared the report with the team, but the team requested more data, so the scientist analyzed it again.");
    std::ostringstream result;
    if (word1.empty() || word2.empty()) {
        result << "Can not be null!";
        return result.str();
    }

    if (graph.find(word1) == graph.end() || graph.find(word2) == graph.end()) {
        result << "No " << word1 << " or " << word2 << " in the graph!";
        return result.str();
    }

    std::vector<std::string> bridges;
    for (const auto& mid: graph.at(word1)) {
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
    for (const auto& pair: graph) nodes.push_back(pair.first);
    std::uniform_int_distribution<> startDist(0, nodes.size() - 1);
    std::string current = nodes[startDist(gen)];

    std::set<std::pair<std::string, std::string>> visitedEdges;
    std::vector<std::string> path;
    path.push_back(current);

    output << "开始随机游走，起点：" << current << "\n";

    while (graph.count(current) && !graph.at(current).empty()) {
        const auto& neighbors = graph.at(current);
        std::vector<std::string> choices;
        for (const auto& [to, _]: neighbors) {
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
            for (const auto& mid: graph.at(words[i])) {
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
    if (graph.count(word)) return true;// 作为起点
    for (const auto& [from, edges]: graph) {
        if (edges.count(word)) return true;// 作为终点
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
        for (const auto& pair: graph) dist[pair.first] = INF;
        dist[start] = 0;

        using P = std::pair<int, std::string>;
        std::priority_queue<P, std::vector<P>, std::greater<>> pq;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            for (const auto& [v, w]: graph.at(u)) {
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
