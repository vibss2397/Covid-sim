#include <unordered_map>
#include <vector>
#include <iostream>
#include <math.h>
#include <functional>
#include "Graph.hpp"


Graph::Graph(std::vector<Person*>& worldPopulation, float p) {
    connectionProb = p;
    population = std::ref(worldPopulation);
    numNodes = population.size();
    initialize_connection_graph();
}

/**
 * @brief Use random graph algorithm: each edge is decided with probability p
 * Considered Babarasi-Albert model to satisfy Power Law (https://en.wikipedia.org/wiki/Scale-free_network), but end up choosing
 * random graph for simplicity and uniform degree
 */
void Graph::initialize_connection_graph() {
    for(int i=0;i<numNodes;i++){
        std::unordered_map<int, float> temp;
        connectionGraph.emplace_back(temp);
    }
    for (int firstNode = 0; firstNode < numNodes; firstNode++) {
        for (int secondNode = firstNode + 1; secondNode < numNodes; secondNode++) {
            if (construct_link_successful()) {
                float linkStrength = new_link_strength();
                
                connectionGraph[firstNode].insert({secondNode, linkStrength});
                connectionGraph[secondNode].insert({firstNode, linkStrength});
            }
        }
    }

}

/// vector shouldn't be copied when returning here via RVO
/// make sure it doesn't copy
std::vector<std::pair<Person*, float>> Graph::get_neighbors(int nodeId) {
    std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
    
    std::vector<std::pair<Person*, float>> neighbors;
    for (auto keyPair : nodeMap) {
        if (keyPair.second > 0) {
            int neighborId = keyPair.first;
            neighbors.emplace_back(population[neighborId], keyPair.second);
        }
    }
    return neighbors;
}

void Graph::quarantine(int nodeId) {
    std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
    for (auto neighborAndValue : nodeMap) {
        int neighbor = neighborAndValue.first;
        nodeMap[neighbor] = 0;
        connectionGraph[neighbor][nodeId] = 0;
    }
}

void Graph::unquarantine(int nodeId) {
    std::unordered_map<int, float>& nodeMap = connectionGraph[nodeId];
    for (auto neighborAndValue : nodeMap) {
        int neighbor = neighborAndValue.first;
        float linkStrength = new_link_strength();
        nodeMap[neighbor] = linkStrength;
        connectionGraph[neighbor][nodeId] = linkStrength;
    }
}

/**
 * @brief Calculate a new link stength according to the power law distribution
 * 
 * @return float 
 */
float Graph::new_link_strength() {
    /// power law distribution range x0 = 0.1, x1 = 1, n = -2.5
    float x0 = 0.1;
    float x1 = 1;
    float n = -2.5;
    /// x = [y * (x1^(n+1) - x0^(n+1)) + x0^(n+1)]^(1/(n+1))
    /// Source: https://mathworld.wolfram.com/RandomNumber.html
    return pow(uniform() * (pow(x1, 1 + n) - pow(x0, 1 + n)) + pow(x0, 1 + n), 1 / (1 + n));
}

bool Graph::construct_link_successful() {
    float temp = uniform();
    return temp<connectionProb;
}

/**
 * @brief Generate a random number uniformly 
 * 
 * @return float 
 */
float Graph::uniform() {
    return (float) rand() / RAND_MAX;
}

void remove_element_from_vector(std::vector<int>& vector, int num) {
    for(std::vector<int>::iterator iter = vector.begin(); iter != vector.end(); ++iter) {
        if (*iter == num) {
            vector.erase(iter);
            break;
        }
    }
}
