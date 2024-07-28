#ifndef MYTEST_TEST_SUITE_H
#define MYTEST_TEST_SUITE_H

#include <string>
#include <vector>
#include "test_case.h"

class TestSuite {
public:
    TestSuite() = default;

    TestSuite(const std::string& name) : name(name) {}

    void addTestCase(const TestCase& testCase) {
        testCases.push_back(testCase);
    }

    const std::string& getName() const { return name; }

    const std::vector<TestCase>& getTestCases() const { return testCases; }

    void run() {
        std::cout << "########## Running test suite: " << name <<" ############" << std::endl;
        std::cout << "----------------------" << std::endl;
        int failed_tests = 0;
        for (auto& testCase : testCases) {
            testCase.run();
            if (testCase.failed()) {
                ++failed_tests;
            }
        }
        std::cout << "Test suite: " << name << std::endl;
        std::cout << "Total tests: " << testCases.size() << std::endl;
        std::cout << "Passed tests: " << testCases.size() - failed_tests << std::endl;
        std::cout << "Failed tests: " << failed_tests << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "#########################################" << std::endl;
    }

private:
    std::string name;
    std::vector<TestCase> testCases;
};

#endif // MYTEST_TEST_SUITE_H
