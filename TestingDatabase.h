#ifndef TestingDatabase_H
#define TestingDatabase_H

#include <vector>
#include "SingleTest.h"

class TestingDatabase
{
    int n_nodes;
    int m_tests;

    // List of list of booleans representing tests results for each node
    std::vector< std::vector<bool> > results;

    SingleTest test;

public:
    // Constructor
    TestingDatabase(int n_nodes, int m_tests);

    // Destructor
    ~TestingDatabase();

    // Function called by the Simulator class to run tests on all nodes
    void run_tests(int iteration);

    // Helper function to check that the results vectors are being updated
    void print_results();
};

#endif
