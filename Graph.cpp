#include <unordered_map>
#include <vector>

class Graph {
    public:
        std::unordered_map<int, float>* connectionGraph; // connections and strength between people
        int numNodes;

    Graph(int requestedNumNodes, int avgDegree) {
        numNodes = requestedNumNodes;
        initialize_connection_graph(avgDegree);
    }

    void initialize_connection_graph(int avgDegree) {
        std::unordered_map<int, float> connectionGraph[numNodes];

        // TODO: initialize the graph with wanted properties
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
            nodeMap[neighbor] = new_link_strength();
            connectionGraph[neighbor][nodeId] = new_link_strength();
        }
    }

    void vary_connection_strength() {
        // TODO: change the connection strength based on some algo?
    }

    float new_link_strength() {
        // TODO: determine strength of a new link after quarantine
        // TBD (maybe a random value based on some metrics?)
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
