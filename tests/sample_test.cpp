#include "../include/mytest/assert.h"
#include "../include/mytest/test.h"
#include "../include/mytest/test_case.h"
#include "../include/mytest/test_suite.h"
#include "../include/mytest//fixture_template.h"
#include "../include/mytest/parameterized_test_params.h" 
#include "../include/mytest/database_fixture.h"
#include <vector>
#include <thread>


// 定义具体的测试夹具类，继承自模板类
class MyTestFixture : public TestFixtureTemplate<MyTestFixture> {
public:
    void SetUp() {
        // 初始化代码
        value = 0;
    }

    void TearDown() {
        // 清理代码
    }

    // 测试夹具中的成员变量
    int value;
};


// 定义具体的数据库测试夹具
class DatabaseTestFixture : public DatabaseFixtureTemplate<DatabaseTestFixture> {
public:
    void SetUpDatabase() {
        db.executeQuery("INSERT INTO users (id, name) VALUES (1, 'John Doe')");
        db.executeQuery("INSERT INTO users (id, name) VALUES (2, 'Jane Smith')");
    }

    void TearDownDatabase() {
        db.executeQuery("DELETE FROM users");
    }
};

TEST_F(DatabaseTestFixture, TestInsertAndQuery) {
    db.executeQuery("SELECT * FROM users");
    ASSERT_TRUE(db.isConnected());
    ASSERT_EQ(db.getQueries().size(), 3);
    ASSERT_EQ(db.getQueries()[0], "INSERT INTO users (id, name) VALUES (1, 'John Doe')");
    ASSERT_EQ(db.getQueries()[1], "INSERT INTO users (id, name) VALUES (2, 'Jane Smith')");
    ASSERT_EQ(db.getQueries()[2], "SELECT * FROM users");
}

TEST_SUITE(SampleSuite);

TEST_IN_SUITE(SampleSuite, SampleTest, TrueIsTrue) {
    ASSERT_TRUE(true);
    return;
}

TEST_IN_SUITE(SampleSuite, SampleTest, FalseIsFalse) {
    ASSERT_FALSE(false);
    return;
}

TEST_SUITE(SampleSuite2);

TEST_IN_SUITE(SampleSuite2, SampleTest, TrueIsTrue2) {
    ASSERT_TRUE(true);
    return;
}

TEST_IN_SUITE(SampleSuite2, SampleTest, FalseIsFalse2) {
    ASSERT_FALSE(false);
    return;
}
/*
* TEST(SampleTest, TrueIsTrue) {
    ASSERT_TRUE(true);
    return 0;
}

TEST(SampleTest, FalseIsFalse) {
    ASSERT_FALSE(false);
    return 0;
}
*/




TEST(SampleTest, EqualInts) {
    ASSERT_EQ(1, 1);
    return;
}

TEST(SampleTest, NotEqualInts) {
    ASSERT_NE(1, 2);
    return;
}

TEST(SampleTest, LessThan) {
    ASSERT_LT(1, 2);
    return;
}

TEST(SampleTest, LessThanOrEqual) {
    ASSERT_LE(1, 1);
    return;
}

TEST(SampleTest, GreaterThan) {
    ASSERT_GT(2, 1);
    return;
}

TEST(SampleTest, GreaterThanOrEqual) {
    ASSERT_GE(1, 2);
    return;
}

TEST(SampleTest, NearEqual) {
    ASSERT_NEAR(1.0, 1.001, 0.01);
    return;
}

TEST(SampleTest, ThrowsException) {
    ASSERT_THROW(throw std::runtime_error("error"), std::runtime_error);
    return;
}

TEST(SampleTest, NoThrowException) {
    ASSERT_NO_THROW(int a = 1 + 1);
    return;
}

TEST(SampleTest, ExpectTest) {
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_EQ(0, 1);
//    EXPECT_NE(1, 2);
    return;
}

TEST_P(ParameterizedTest, Equals, int) {
    ASSERT_TRUE(param < 3);
}
TEST(SampleTest, DurationLessThan) {
    ASSERT_DURATION_LESS_THAN(100, {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate some work
        });
    return;
}

// 使用测试夹具
TEST_F(MyTestFixture, TestWithFixture) {
    value = 42;
    ASSERT_EQ(42, value);
}


int main() {
    UnitTest& unit_test = UnitTest::getInstance();

    ParameterizedTest_Equals_Params = { 1, 2, 3, 4, 5 }; // 设置测试参数
    return unit_test.runAllTests();
}