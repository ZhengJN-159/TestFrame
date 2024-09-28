#ifndef MYTEST_UNIT_TEST_H
#define MYTEST_UNIT_TEST_H

#include <vector>
#include <unordered_map>
#include <functional>
#include "test_case.h"
#include "test_suite.h"
#include <map>
#include <fstream>
#include <ctime>

class UnitTest {
public:
    static UnitTest& getInstance() {
        static UnitTest instance;
        return instance;
    }

    void addTestCase(const TestCase& testCase) {
        testCases.push_back(testCase);
    }

    void addTestSuite(const std::string& suiteName) {
        if (testSuites.find(suiteName) == testSuites.end()) {
            testSuites.emplace(suiteName, TestSuite(suiteName));
        }
    }

    void addTestCaseToSuite(const std::string& suiteName, const TestCase& testCase) {
        testSuites[suiteName].addTestCase(testCase);
    }
    //template<typename ParamType>
    //void addParameterizedTest(const std::string& testName, const std::function<void(ParamType)>& func) {
    //    addTestCase(TestCase(testName, [=]() { func(ParamType()); })); // 或者传入有效参数
    //}
    void addParameterizedTest(const std::string& testName, const std::function<void()>& func) {
        // 将参数化测试作为普通测试用例添加
        addTestCase(TestCase(testName, func));
    }

    void addParameterizedTestToSuite(const std::string& suiteName, const std::string& testName, const std::function<void()>& func) {
        if (testSuites.find(suiteName) == testSuites.end()) {
            addTestSuite(suiteName);
        }
        testSuites[suiteName].addTestCase(TestCase(testName, func));
    }



    int runAllTests(const std::string& file_path) {
        int failed_tests = 0;
        int total_tests = 0;

        std::ofstream report(file_path);
        auto now = std::time(nullptr);
        report << "Test Report - " << std::ctime(&now) << "\n";
        report << "=============================\n";

        //Run individual test cases
        for (auto& testCase : testCases) {
            testCase.run();
            ++total_tests;
            if (testCase.failed()) {
                ++failed_tests;
            }
            recordTestCase(report, testCase);
        }

   

        // Run test suites
        for (auto& suitePair : testSuites) {
            suitePair.second.run();
            for (const auto& testCase : suitePair.second.getTestCases()) {
                ++total_tests;
                if (testCase.failed()) {
                    ++failed_tests;
                }
                recordTestCase(report, testCase);
            }
        }

        report << "Total tests: " << total_tests << "\n";
        report << "Passed tests: " << total_tests - failed_tests << "\n";
        report << "Failed tests: " << failed_tests << "\n";
        report.close();


        std::cout << "Total tests: " << total_tests << std::endl;
        std::cout << "Total test suites: " << testSuites.size() << std::endl;
        std::cout << "Passed tests: " << total_tests - failed_tests << std::endl;
        std::cout << "Failed tests: " << failed_tests << std::endl;
        system("pause");
        return failed_tests;
    }

private:
    std::vector<TestCase> testCases;
    std::unordered_map<std::string, TestSuite> testSuites;

    void recordTestCase(std::ofstream& report, const TestCase& testCase) {
        report << "Test Case: " << testCase.getName() << "\n";
        if (testCase.failed()) {
            report << "Status: Failed\n";
            report << "Failures:\n";
            for (const auto& failure : testCase.getFailureMessage()) {
                report << "  - " << failure << "\n";
            }
        }
        else {
            report << "Status: Passed\n";
        }
        report << "----------------------\n";
    }
    UnitTest() = default;
};

#endif // MYTEST_UNIT_TEST_H
