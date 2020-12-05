#include <iostream>
#include <random>
#include "SingleTest.h"

/* A class that represents a single COVID test */

SingleTest::SingleTest()
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

    // How likely people who are positive test positive
    sensitivity = 0.84;

    // How likely people who are negative test negative
    specificity = 1.00;
}

SingleTest::~SingleTest()
{
}

/* This function returns a test result for a given node. It's given a node's id
 * and it interfaces with the Graph(?) class to get data about that node's
 * infected status. */
bool SingleTest::test_node(int id)
{
    // DUMMY NODE, DELETE LATER!!!
    std::vector<bool> node = {true};

    // Initialize the result to node's actual infection status
    bool status = node[0];
    bool result = status;

    // If the node is not infected, the PCR test is unlikely to report a false positive
    if (!status) { return result; }

    // Generate a random number and see if it's greater than the PCR test's sensitivity
    // Used the following as reference:
    // https://www.delftstack.com/howto/cpp/how-to-generate-random-float-number-in-cpp/
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(0, 1);
    float random = distr(eng);

    // If the node is infected, the test could return a false negative
    if (random > sensitivity)
    {
        result = !result;
    }

    return result;

}
