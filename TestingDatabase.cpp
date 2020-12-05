#include <vector>
#include <random>
#include <iostream>
#include "TestingDatabase.h"
#include "SingleTest.h"

/* A class which represents a database of test results. */

TestingDatabase::TestingDatabase(int n_nodes, int m_tests):
n_nodes(n_nodes), m_tests(m_tests)
{
    // Initialize the results vector to hold n_nodes vectors, each
    // with m_tests booleans
    this->results.resize(n_nodes, std::vector<bool>(m_tests));

    // Create an instance of the SingleTest class,
    // which contains the method for testing a given node
    SingleTest test = SingleTest();
}

TestingDatabase::~TestingDatabase()
{
}

/* This method is called by the Simulator class when it wants
 * to run tests on the nodes within the population. It needs to know
 * which iteration the tests are for so the results can be stored
 * in the results matrix correctly. */
void TestingDatabase::run_tests(int iteration)
{

    // Conduct a test for each node
    for (int i = 0; i < n_nodes; i++)
    {
        bool result = test.test_node(i);
        results[i][iteration] = result;
    }
    
}

/* Helper function that prints out the results vectors for each node */
void TestingDatabase::print_results()
{
     for (int i = 0; i < n_nodes; i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < m_tests; j++)
        {
            std::cout << results[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
