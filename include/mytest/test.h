#ifndef MYTEST_TEST_H
#define MYTEST_TEST_H

#include "unit_test.h"
#include <vector>

#define TEST(test_case_name, test_name) \
    void test_case_name##_##test_name(); \
    namespace { \
        struct test_case_name##_##test_name##_Registrator { \
            test_case_name##_##test_name##_Registrator() { \
                UnitTest::getInstance().addTestCase(TestCase(#test_case_name "." #test_name, test_case_name##_##test_name)); \
            } \
        }; \
        static test_case_name##_##test_name##_Registrator test_case_name##_##test_name##_registrator; \
    } \
    void test_case_name##_##test_name()

#define TEST_SUITE(suite_name) \
    namespace suite_name##_ns { \
        struct suite_name##_Registrator { \
            suite_name##_Registrator() { \
                UnitTest::getInstance().addTestSuite(#suite_name); \
            } \
        }; \
        static suite_name##_Registrator suite_name##_registrator; \
    }

#define TEST_IN_SUITE(suite_name, test_case_name, test_name) \
    void test_case_name##_##test_name(); \
    namespace { \
        struct test_case_name##_##test_name##_Registrator { \
            test_case_name##_##test_name##_Registrator() { \
                UnitTest::getInstance().addTestCaseToSuite(#suite_name, TestCase(#test_case_name "." #test_name, test_case_name##_##test_name)); \
            } \
        }; \
        static test_case_name##_##test_name##_Registrator test_case_name##_##test_name##_registrator; \
    } \
    void test_case_name##_##test_name()

//#define TEST_P(test_case_name, test_name, param_type) \
//    void test_case_name##_##test_name##_Func(param_type param); \
//    namespace { \
//        struct test_case_name##_##test_name##_Registrator { \
//            test_case_name##_##test_name##_Registrator() { \
//                UnitTest::getInstance().addParameterizedTest(#test_case_name "." #test_name, [=]() { \
//                    for (const auto& param : get_##test_case_name##_##test_name##_Params()) { \
//                        test_case_name##_##test_name##_Func(param); \
//                    } \
//                }); \
//            } \
//        }; \
//        static test_case_name##_##test_name##_Registrator test_case_name##_##test_name##_registrator; \
//        std::vector<param_type>& get_##test_case_name##_##test_name##_Params(); \
//    } \
//    void test_case_name##_##test_name##_Func(param_type param)

#define TEST_P(test_case_name, test_name, param_type) \
    void test_case_name##_##test_name##_Func(param_type param); \
    namespace { \
        struct test_case_name##_##test_name##_Registrator { \
            test_case_name##_##test_name##_Registrator() { \
                UnitTest::getInstance().addParameterizedTest(#test_case_name "." #test_name, [=]() { \
                    for (const auto& param : get_##test_case_name##_##test_name##_Params()) { \
                        test_case_name##_##test_name##_Func(param); \
                    } \
                }); \
            } \
        }; \
        static test_case_name##_##test_name##_Registrator test_case_name##_##test_name##_registrator; \
        std::vector<param_type>& get_##test_case_name##_##test_name##_Params(); \
    } \
    void test_case_name##_##test_name##_Func(param_type param)

// ÃÌº”≤‚ ‘º–æﬂ∫Í
#define TEST_F(fixture_name, test_name) \
    class fixture_name##_##test_name##_Test : public fixture_name { \
    public: \
        void TestBody(); \
        static void RunTest() { \
            fixture_name##_##test_name##_Test instance; \
            instance.SetUp(); \
            instance.TestBody(); \
            instance.TearDown(); \
        } \
    }; \
    namespace { \
        struct fixture_name##_##test_name##_Registrator { \
            fixture_name##_##test_name##_Registrator() { \
                UnitTest::getInstance().addTestCase(TestCase(#fixture_name "." #test_name, fixture_name##_##test_name##_Test::RunTest)); \
            } \
        }; \
        static fixture_name##_##test_name##_Registrator fixture_name##_##test_name##_registrator; \
    } \
    void fixture_name##_##test_name##_Test::TestBody()

#endif // MYTEST_TEST_H
