#ifndef MYTEST_TEST_CASE_H
#define MYTEST_TEST_CASE_H

#include <windows.h>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <chrono>
#include "console_color.h"

class TestCase;
extern TestCase* currentTestCase;

class TestCase {
public:
    TestCase(const std::string& name, std::function<void()> func)
        : name(name), func(func), test_failed(false) {}

    const std::string& getName() const { return name; }
    std::function<void()> getFunction() const { return func; }
    const std::vector<std::string> getFailureMessage() const{ return failures; }

    void run() {
        currentTestCase = this;
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Running test: " << name << std::endl;
        func();
        currentTestCase = nullptr;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        currentTestCase = nullptr;

        if (test_failed) {
#ifdef _WIN32
            setConsoleColor(FOREGROUND_RED);
#else
            setConsoleColor("\033[31m"); // 红色
#endif
            std::cout << "Test " << name << " failed." << std::endl;
            printFailures();
        }
        else {
#ifdef _WIN32
            setConsoleColor(FOREGROUND_GREEN);
#else
            setConsoleColor("\033[32m"); // 绿色
#endif
            std::cout << "Test " << name << " passed." << std::endl;
        }
        std::cout << "Duration: " << duration.count() << " seconds." << std::endl;
        std::cout << "----------------------" << std::endl;
#ifdef _WIN32
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
        setConsoleColor("\033[0m"); // 重置颜色
#endif
    }

    void addFailure(const std::string& message) {
        test_failed = true;
        failures.push_back(message);
    }

    bool failed() const { return test_failed; }

    void printFailures() const {
        for (const auto& failure : failures) {
            std::cerr << failure << std::endl;
        }
    }

private:
    std::string name;
    std::function<void()> func;
    bool test_failed;
    std::vector<std::string> failures;
};

// 添加测试夹具支持
class TestFixture {
public:
    virtual ~TestFixture() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

template<typename T>
class TestWithFixture : public TestCase, public T {
public:
    TestWithFixture(const std::string& name, std::function<void(T&)> func)
        : TestCase(name, [this, func]() {
        this->SetUp();
        func(*this);
        this->TearDown();
            }) {}
};


#endif // MYTEST_TEST_CASE_H