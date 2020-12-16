#ifndef TestingDatabase_H
#define TestingDatabase_H

#include <vector>

/**
 * @brief This class models a rapid antigen test; It is called by an instance of the Person class and will return a test result based on that person’s COVID status (eg. COVID-free, infected, symptomatic, quarantined, etc.).
 * 
 */
class TestingDatabase
{
    int n_nodes;
    int m_tests;

    float symptomatic_sensitivity;
    float specificity;
    float asymptomatic_sensitivity;

public:
    ///Constructor
    TestingDatabase(int n_nodes, int m_tests);

    ///Destructor
    ~TestingDatabase();

    ///Function called by the Simulator class to run tests on all nodes
    ///void run_tests(int iteration);

    ////Function to be called by the Simulator class based on testing frequency
    bool test_node(int bu_id, int person_state);

    ///Helper function to check that the results vectors are being updated
    void print_results();

    ///List of list of booleans representing tests results for each node
    std::vector<std::vector<bool>> results;

    ///List of ints representing which index to store a Person's next test
    ///at within the results matrix. I did this to avoid using linked lists
    ///for the results.
    std::vector<int> test_num;
};

#endif
