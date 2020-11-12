#include <iostream>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <unordered_set>

class undirectedGraph {
private:
    std::unordered_map<int, std::unordered_set<int>> nodes;
    std::unordered_set<int> v;

    void dfs(int key, std::unordered_set<int> &visited) {
        visited.insert(key);
        v.insert(key);
        for (const auto &it : nodes[key])
            if (visited.find(it) == visited.end())
                dfs(it, visited);
    }

public:
    undirectedGraph() = default;

    ~undirectedGraph() {};

    void insertVertex(int key) {
        if (nodes.find(key) == nodes.end())
            nodes[key] = std::unordered_set<int>();
    }

    void insertEdge(int start_node_key, int end_node_key) {
        nodes[start_node_key].insert(end_node_key);
        nodes[end_node_key].insert(start_node_key);
    }

    void printGraph() {
        for (const auto &it: nodes) {
            std::cout << "key:" << it.first << ", edges ->";
            for (const auto &it2 : it.second)
                std::cout << " {key:" << it2 << "} ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    int connectedComponents() {
        int contConnected = 0;
        std::unordered_set<int> visited;
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (visited.find(it->first) == visited.end()) {
                dfs(it->first, visited);
                contConnected += v.size() - 1;
                v.clear();
            }
        }
        return contConnected;
    }
};
