#include "Person.hpp"
#include "Graph.hpp"
#include <vector>
#include <iostream>
int main(){
    std::vector<Person> p(10);
    for(int i = 0; i<10;i++){
        Person person = Person(i);
        p[i] = person;
    }
    Graph g = Graph(p, 0.8f);
    g.get_neighbors(1);
}