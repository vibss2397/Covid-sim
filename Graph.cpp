#include <unordered_map>
#include <vector>

class Graph {
    public:
        std::unordered_map<int, float>* connectionGraph; // connections and strength between people
        int numNodes;

    Graph(int requestedNumNodes, float p) {
        numNodes = requestedNumNodes;
        initialize_connection_graph(p);
    }

    // Use random graph algorithm: each edge is decided with probability p
    // Considered Babarasi-Albert model to satisfy Power Law (https://en.wikipedia.org/wiki/Scale-free_network), but end up choosing
    // random graph for simplicity and uniform degree
    void initialize_connection_graph(float p) {
        std::unordered_map<int, float> connectionGraph[numNodes];
        
        for (int firstNode = 0; firstNode < numNodes; firstNode++) {
            for (int secondNode = firstNode + 1; secondNode < numNodes; secondNode++) {
                if (construct_link_successful(p)) {
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
        // power law distribution
    }

    bool construct_link_successful(float p) {
        float ranNum = rand() / RAND_MAX;
        return ranNum < p;
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
