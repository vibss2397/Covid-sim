#include <unordered_map>
#include <vector>
#include <iostream>
#include <math.h>

class Graph {
    public:
        std::unordered_map<int, float>* connectionGraph; // connections and strength between people
        int numNodes;
        float connectionProb;

    Graph(int requestedNumNodes, float p) {
        numNodes = requestedNumNodes;
        connectionProb = p;
        initialize_connection_graph();
    }

    // Use random graph algorithm: each edge is decided with probability p
    // Considered Babarasi-Albert model to satisfy Power Law (https://en.wikipedia.org/wiki/Scale-free_network), but end up choosing
    // random graph for simplicity and uniform degree
    void initialize_connection_graph() {
        std::unordered_map<int, float> connectionGraph[numNodes];
        
        for (int firstNode = 0; firstNode < numNodes; firstNode++) {
            for (int secondNode = firstNode + 1; secondNode < numNodes; secondNode++) {
                if (construct_link_successful()) {
                    float linkStrength = new_link_strength();
                    connectionGraph[firstNode][secondNode] = linkStrength;
                    connectionGraph[secondNode][firstNode] = linkStrength;
                }
            }
        }
    }

    // vector shouldn't be copied when returning here via RVO
    // make sure it doesn't copy
    std::vector<int> get_neighbors(int nodeId) {
        std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
        std::vector<int> neighbors(nodeMap.size());
        for (auto keyPair : nodeMap) {
            if (keyPair.second > 0) {
                neighbors.push_back(keyPair.first);
            }
        }
        return neighbors;
    }

    void quarantine(int nodeId) {
        std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
        for (auto neighborAndValue : nodeMap) {
            int neighbor = neighborAndValue.first;
            nodeMap[neighbor] = 0;
            connectionGraph[neighbor][nodeId] = 0;
        }
    }

    void unquarantine(int nodeId) {
        std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
        for (auto neighborAndValue : nodeMap) {
            int neighbor = neighborAndValue.first;
            float linkStrength = new_link_strength();
            nodeMap[neighbor] = linkStrength;
            connectionGraph[neighbor][nodeId] = linkStrength;
        }
    }

    void vary_connection_strength() {
        // TODO: change the connection strength based on some algo?
    }

    float new_link_strength() {
        // power law distribution range x0 = 0.1, x1 = 1, n = -2.5
        float x0 = 0.1;
        float x1 = 1;
        float n = -2.5;
        // x = [y * (x1^(n+1) - x0^(n+1)) + x0^(n+1)]^(1/(n+1))
        // Source: https://mathworld.wolfram.com/RandomNumber.html
        return pow(uniform() * (pow(x1, 1 + n) - pow(x0, 1 + n)) + pow(x0, 1 + n), 1 / (1 + n));
    }

    bool construct_link_successful() {
        return uniform() < connectionProb;
    }

    float uniform() {
        return (float) rand() / RAND_MAX;
    }
};

void remove_element_from_vector(std::vector<int>& vector, int num) {
    for(std::vector<int>::iterator iter = vector.begin(); iter != vector.end(); ++iter) {
        if (*iter == num) {
            vector.erase(iter);
            break;
        }
    }
}
