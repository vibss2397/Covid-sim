#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>
#include <vector>
#include "Person.hpp"

class Graph {
    public:
        std::unordered_map<int, float>* connectionGraph; // connections and strength between people
        int numNodes;
        float connectionProb;
        std::vector<Person> population;

    Graph(std::vector<Person>& worldPopulation, float p);

    void initialize_connection_graph();

    std::vector<std::pair<Person, float>> get_neighbors(int nodeId);

    void quarantine(int nodeId);

    void unquarantine(int nodeId);

    void vary_connection_strength();

    float new_link_strength();

    bool construct_link_successful();

    float uniform();

};

#endif
