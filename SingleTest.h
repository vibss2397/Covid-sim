#ifndef SingleTest_H
#define SingleTest_H

class SingleTest
{

float sensitivity;
float specificity;

public:
    SingleTest();
    ~SingleTest();

    // Function to be called by an isntance of TestingDatabase
    bool test_node(int id);
};

#endif
