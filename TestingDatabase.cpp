#include <vector>
#include <random>
#include <iostream>
#include "TestingDatabase.h"

/**
 * @brief Construct a new Testing Database:: Testing Database object, A class which represents a database of test results.
 * 
 * @param n_nodes 
 * @param m_tests 
 */
TestingDatabase::TestingDatabase(int n_nodes, int m_tests):
n_nodes(n_nodes), m_tests(m_tests)
{
    // Data about rapid antigen tests taken from:
    // https://www.cdc.gov/coronavirus/2019-ncov/lab/resources/antigen-tests-guidelines.html
    // Quote from the source:
    // "The sensitivity of rapid antigen tests is generally lower than RT-PCR.
    // The first antigen tests to have received FDA EUAs demonstrate sensitivity
    // ranging from 84.0%-97.6% compared to RT-PCR. ...
    // The specificity of rapid antigen tests is generally as high as RT-PCR –
    // the first antigen tests that have received FDA EUAs have reported
    // specificity of 100% – which means that false positive results are unlikely."

    // How likely people who are positive test positive.
    symptomatic_sensitivity = 0.84;

    // How likely people who are negative test negative.
    specificity = 1.00;

    // How likely people who are infected, but not yet symptomatic, test positive.
    asymptomatic_sensitivity = 0.50;


    // Initialize the results vector to hold n_nodes vectors, each
    // with m_tests booleans.
    this->results.resize(n_nodes, std::vector<bool>(m_tests));

    // Initialize the test_num vector to keep track of which
    // test number a node is up to.
    this->test_num.resize(n_nodes); 
}

TestingDatabase::~TestingDatabase()
{
}

/* This function returns a test result for a given person.
 * It's called by the Simulator instance and is given the node's state:
 * 0: safe(potential), 1 : infected, 2 : symptomatic, 3 : quarantine, 4: recovery, -1 : dead */
bool TestingDatabase::test_node(int bu_id, int person_state)
{
    bool result;
    int test_num = this->test_num[bu_id];

    if (person_state <= 0) {
        // If the node is not infected, the PCR test is unlikely
        // to report a false positive.
        result = false;
    }
    else {
        // Generate a random number and see if it's greater than the PCR test's sensitivity
        // Used the following as reference:
        // https://www.delftstack.com/howto/cpp/how-to-generate-random-float-number-in-cpp/
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<> distr(0, 1);
        float random = distr(eng);

        if (person_state == 1) {
            // If person a person has just been infected, it's less likely
            // they will test positive right away.
            if (random > asymptomatic_sensitivity) {
                result = true;
            }

        }
        else if (person_state == 2 || person_state == 3) {
            // If the node is infected, the test could return a false negative
            if (random > symptomatic_sensitivity)
            {
                result = !result;
            }
        }
    }

    results[bu_id][test_num] = result;
    this->test_num[bu_id]++;

    return result;

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
