// include/mytest/assert.h
#ifndef MYTEST_ASSERT_H
#define MYTEST_ASSERT_H

#include <iostream>
#include "test_case.h"
#include "unit_test.h"
#include "mock.h"

#ifdef _WIN32
#define SET_CONSOLE_COLOR_RED() setConsoleColor(FOREGROUND_RED)
#define SET_CONSOLE_COLOR_RESET() setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#else
#define SET_CONSOLE_COLOR_RED() setConsoleColor("\033[31m") /* 红色 */
#define SET_CONSOLE_COLOR_RESET() setConsoleColor("\033[0m") /* 重置颜色 */
#endif

// ASSERT 宏
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #condition); \
        } \
        return; \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #condition " is true"); \
        } \
        return; \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: expected " #expected ", but got " #actual); \
        } \
        return; \
    }

#define ASSERT_NE(val1, val2) \
    if ((val1) == (val2)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " == " #val2); \
        } \
        return; \
    }

#define ASSERT_LT(val1, val2) \
    if (!((val1) < (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " >= " #val2); \
        } \
        return; \
    }

#define ASSERT_LE(val1, val2) \
    if (!((val1) <= (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " > " #val2); \
        } \
        return; \
    }

#define ASSERT_GT(val1, val2) \
    if (!((val1) > (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " <= " #val2); \
        } \
        return; \
    }

#define ASSERT_GE(val1, val2) \
    if (!((val1) >= (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " < " #val2); \
        } \
        return; \
    }

#define ASSERT_NEAR(val1, val2, abs_error) \
    if (std::fabs((val1) - (val2)) > (abs_error)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #val1 " and " #val2 " are not within " #abs_error); \
        } \
        return; \
    }

#define ASSERT_THROW(statement, exception_type) \
    try { \
        statement; \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: expected exception " #exception_type " not thrown"); \
        } \
        return; \
    } catch (const exception_type&) { \
    } catch (...) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: expected exception " #exception_type " but got a different type of exception"); \
        } \
        return; \
    }

#define ASSERT_NO_THROW(statement) \
    try { \
        statement; \
    } catch (...) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: unexpected exception thrown"); \
        } \
        return; \
    }


// String assertions
#define ASSERT_STREQ(expected, actual) \
    if (strcmp((expected), (actual)) != 0) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: expected " #expected " == " #actual); \
        } \
        return; \
    }

#define ASSERT_STRNE(str1, str2) \
    if (strcmp((str1), (str2)) == 0) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #str1 " != " #str2); \
        } \
        return; \
    }

#define ASSERT_STRCASEEQ(expected, actual) \
    if (!stricmp((expected), (actual))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: expected (case-insensitive) " #expected " == " #actual); \
        } \
        return; \
    }

#define ASSERT_STRCASENE(str1, str2) \
    if (stricmp((str1), (str2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: (case-insensitive) " #str1 " != " #str2); \
        } \
        return; \
    }

#define ASSERT_NULL(ptr) \
    if ((ptr) != nullptr) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #ptr " is not null"); \
        } \
        return; \
    }

#define ASSERT_NOT_NULL(ptr) \
    if ((ptr) == nullptr) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #ptr " is null"); \
        } \
        return; \
    }

#define ASSERT_CONTAINS(container, element) \
    if (std::find((container).begin(), (container).end(), (element)) == (container).end()) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Assertion failed: " #element " not found in " #container); \
        } \
        return; \
    }

#define ASSERT_DURATION_LESS_THAN(duration, code) \
    { \
        auto start = std::chrono::high_resolution_clock::now(); \
        code; \
        auto end = std::chrono::high_resolution_clock::now(); \
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > (duration)) { \
            if (currentTestCase) { \
                currentTestCase->addFailure("Assertion failed: duration of " #code " exceeded " #duration " milliseconds"); \
            } \
            return; \
        } \
    }

//EXPECT

#define EXPECT_TRUE(condition) \
    if (!(condition)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #condition); \
        } else { \
            std::cerr << "Expectation failed: " #condition << std::endl; \
        } \
    }

#define EXPECT_FALSE(condition) \
    if ((condition)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #condition " is true"); \
        } else { \
            std::cerr << "Expectation failed: " #condition " is true" << std::endl; \
        } \
    }

#define EXPECT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: expected " #expected ", but got " #actual); \
        } else { \
            std::cerr << "Expectation failed: expected " #expected ", but got " #actual << std::endl; \
        } \
    }
#define EXPECT_NE(val1, val2) \
    if ((val1) == (val2)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " == " #val2); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " == " #val2 << std::endl; \
        } \
    }

#define EXPECT_LT(val1, val2) \
    if (!((val1) < (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " < " #val2); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " < " #val2 << std::endl; \
        } \
    }

#define EXPECT_LE(val1, val2) \
    if (!((val1) <= (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " <= " #val2); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " <= " #val2 << std::endl; \
        } \
    }

#define EXPECT_GT(val1, val2) \
    if (!((val1) > (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " > " #val2); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " > " #val2 << std::endl; \
        } \
    }

#define EXPECT_GE(val1, val2) \
    if (!((val1) >= (val2))) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " >= " #val2); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " >= " #val2 << std::endl; \
        } \
    }

#define EXPECT_NEAR(val1, val2, abs_error) \
    if (std::fabs((val1) - (val2)) > (abs_error)) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #val1 " and " #val2 " are not within " #abs_error); \
        } else { \
            std::cerr << "Expectation failed: " #val1 " and " #val2 " are not within " #abs_error << std::endl; \
        } \
    }

#define EXPECT_THROW(statement, exception_type) \
    try { \
        statement; \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: expected exception " #exception_type " not thrown"); \
        } else { \
            std::cerr << "Expectation failed: expected exception " #exception_type " not thrown" << std::endl; \
        } \
    } catch (const exception_type&) { \
    } catch (...) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: expected exception " #exception_type " but got a different type of exception"); \
        } else { \
            std::cerr << "Expectation failed: expected exception " #exception_type " but got a different type of exception" << std::endl; \
        } \
    }

#define EXPECT_NO_THROW(statement) \
    try { \
        statement; \
    } catch (...) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: unexpected exception thrown"); \
        } else { \
            std::cerr << "Expectation failed: unexpected exception thrown" << std::endl; \
        } \
    }

#define EXPECT_STREQ(expected, actual) \
    if (strcmp((expected), (actual)) != 0) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: expected " #expected " == " #actual); \
        } else { \
            std::cerr << "Expectation failed: expected " #expected " == " #actual << std::endl; \
        } \
    }

#define EXPECT_STRNE(str1, str2) \
    if (strcmp((str1), (str2)) == 0) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #str1 " != " #str2); \
        } else { \
            std::cerr << "Expectation failed: " #str1 " != " #str2 << std::endl; \
        } \
    }

#define EXPECT_DOUBLE_EQ(expected, actual) \
    if (std::fabs((expected) - (actual)) > std::numeric_limits<double>::epsilon()) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: expected " #expected " == " #actual); \
        } else { \
            std::cerr << "Expectation failed: expected " #expected " == " #actual << std::endl; \
        } \
    }

#define EXPECT_NULL(ptr) \
    if ((ptr) != nullptr) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #ptr " is not null"); \
        } else { \
            std::cerr << "Expectation failed: " #ptr " is not null" << std::endl; \
        } \
    }

#define EXPECT_NOT_NULL(ptr) \
    if ((ptr) == nullptr) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #ptr " is null"); \
        } else { \
            std::cerr << "Expectation failed: " #ptr " is null" << std::endl; \
        } \
    }

#define EXPECT_CONTAINS(container, element) \
    if (std::find((container).begin(), (container).end(), (element)) == (container).end()) { \
        if (currentTestCase) { \
            currentTestCase->addFailure("Expectation failed: " #element " not found in " #container); \
        } else { \
            std::cerr << "Expectation failed: " #element " not found in " #container << std::endl; \
        } \
    }

#define EXPECT_DURATION_LESS_THAN(duration, code) \
    { \
        auto start = std::chrono::high_resolution_clock::now(); \
        code; \
        auto end = std::chrono::high_resolution_clock::now(); \
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > (duration)) { \
            if (currentTestCase) { \
                currentTestCase->addFailure("Expectation failed: duration of " #code " exceeded " #duration " milliseconds"); \
            } else { \
                std::cerr << "Expectation failed: duration of " #code " exceeded " #duration " milliseconds" << std::endl; \
            } \
        } \
    }


#define EXPECT_CALL(mock_obj, method) \
    mock_obj.expect_call(#method)

#define WILL_RETURN(mock_obj, method, returnValue) \
    mock_obj.will_return(#method, returnValue)

#define WILL_THROW(mock_obj, method, exception) \
    mock_obj.will_throw(#method, exception)



//template<typename T>
//std::any to_any(const T& value) {
//    return std::any(value);
//}
//
//template<typename... Args>
//std::vector<std::any> make_any_vector(Args&&... args) {
//    return { to_any(std::forward<Args>(args))... };
//}
//#define MOCK_METHOD(methodName, returnType, ...) \
//    returnType methodName(__VA_ARGS__) override { \
//        std::vector<std::any> args = { __VA_ARGS__ }; \
//        this->recordCall(#methodName, args); \
//        return this->get_return_value<returnType>(#methodName); \
//    }



 //定义不同参数数量的宏
#define MOCK_METHOD0(methodName, returnType) \
    returnType methodName() override { \
        this->recordCall(#methodName, std::vector<std::any>{}); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD1(methodName, returnType, arg1Type, arg1Name) \
    returnType methodName(arg1Type arg1Name) override { \
        std::vector<std::any> args = {std::any(arg1Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD2(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name) override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD3(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name) override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD4(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name) override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD5(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, arg5Type arg5Name) override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name), std::any(arg5Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_METHOD6(methodName, returnType, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, arg5Type arg5Name, arg6Type arg6Name) override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name), std::any(arg5Name), std::any(arg6Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD0(methodName, returnType) \
    returnType methodName() const override { \
        this->recordCall(#methodName, std::vector<std::any>{}); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD1(methodName, returnType, arg1Type, arg1Name) \
    returnType methodName(arg1Type arg1Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD2(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD3(methodName, returnType, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }
#define MOCK_CONST_METHOD4(methodName, returnType, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD5(methodName, returnType, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, arg5Type arg5Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name), std::any(arg5Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }

#define MOCK_CONST_METHOD6(methodName, returnType, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name) \
    returnType methodName(arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, arg5Type arg5Name, arg6Type arg6Name) const override { \
        std::vector<std::any> args = {std::any(arg1Name), std::any(arg2Name), std::any(arg3Name), std::any(arg4Name), std::any(arg5Name), std::any(arg6Name)}; \
        this->recordCall(#methodName, args); \
        return this->get_return_value_impl<returnType>(#methodName); \
    }


#endif // MYTEST_ASSERT_H
