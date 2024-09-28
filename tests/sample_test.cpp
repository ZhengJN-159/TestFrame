#include <string>
#include "../include/mytest.h"
#include "../include/mytest/database_fixture.h"
using namespace std;

//1 简单使用
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

//2 数据驱动
using ParamType = std::map<std::string, int>;
TEST_P(MyTest, ComplexEquals, ParamType, "test_data.json", DataLoader::loadJSON) {
    int param1 = param.at("param1");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    EXPECT_EQ(param1 + param2, param3)
    // 测试逻辑：检查 param1 + param2 是否等于 param3
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}

// 使用 CSV 数据进行测试
TEST_P(MyTest, ComplexEquals_CSV, ParamType, "test_data.csv", DataLoader::loadCSV) {
    int param1 = param.at("param1");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    // 测试逻辑
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}

// 使用 XML 数据进行测试
TEST_P(MyTest, ComplexEquals_XML, ParamType, "test_data.xml", DataLoader::loadXML) {
    int param1 = param.at("paramA");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    // 测试逻辑
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}




//3 测试夹具
// 被测试的类
class Calculate {
public:
    int Add(int a, int b) const { return a + b; }
    int Subtract(int a, int b) const { return a - b; }
    int Multiply(int a, int b) const { return a * b; }
    double Divide(int a, int b) const {
        if (b == 0) throw std::invalid_argument("Division by zero");
        return static_cast<double>(a) / b;
    }
};

class Statistics {
public:
    double Mean(const std::vector<int>& data) const {
        if (data.empty()) throw std::invalid_argument("Empty data");
        double sum = std::accumulate(data.begin(), data.end(), 0);
        return sum / data.size();
    }

    double StandardDeviation(const std::vector<int>& data) const {
        if (data.empty()) throw std::invalid_argument("Empty data");
        double mean = Mean(data);
        double accum = 0.0;
        for (int value : data) {
            accum += (value - mean) * (value - mean);
        }
        return std::sqrt(accum / data.size());
    }
};

// 测试夹具
class MyComplexFixture : public TestFixtureTemplate<MyComplexFixture>{
public:
    void SetUp() override {
        // 初始化Calculator和Statistics对象
        calculator = new Calculate();
        statistics = new Statistics();
    }

    void TearDown() override {
        // 释放资源
        delete calculator;
        delete statistics;
    }

    Calculate* calculator;
    Statistics* statistics;
};

// 测试Calculator的Add方法
TEST_F(MyComplexFixture, CalculatorAddTest) {
    EXPECT_EQ(7, calculator->Add(3, 4));
    EXPECT_EQ(calculator->Add(-1, 5), 4);
}

// 测试Calculator的Divide方法
TEST_F(MyComplexFixture, CalculatorDivideTest) {
    EXPECT_DOUBLE_EQ(calculator->Divide(10, 2), 5.0);
    EXPECT_THROW(calculator->Divide(1, 0), std::invalid_argument);
}

// 测试Statistics的Mean方法
TEST_F(MyComplexFixture, StatisticsMeanTest) {
    std::vector<int> data = { 1, 2, 3, 4, 5 };
    EXPECT_DOUBLE_EQ(statistics->Mean(data), 3.0);
    EXPECT_THROW(statistics->Mean({}), std::invalid_argument);
}

// 测试Statistics的StandardDeviation方法
TEST_F(MyComplexFixture, StatisticsStandardDeviationTest) {
    std::vector<int> data = { 1, 2, 3, 4, 5 };
    EXPECT_NEAR(statistics->StandardDeviation(data), 1.41421, 1e-5);
    EXPECT_THROW(statistics->StandardDeviation({}), std::invalid_argument);
}

// 测试Calculator和Statistics的交互
TEST_F(MyComplexFixture, CalculatorAndStatisticsInteractionTest) {
    std::vector<int> data = { calculator->Add(1, 2), calculator->Add(3, 4), calculator->Add(5, 6) };
    EXPECT_DOUBLE_EQ(statistics->Mean(data), 7.0);
    EXPECT_NEAR(statistics->StandardDeviation(data), 2.44949, 1e-5);
}




//4 测试套件
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



//5 接口测试
class Parent {
public:
    virtual ~Parent() {}

    virtual int getNum() const = 0;
    virtual void setResult(int value) = 0;
    virtual void print(const string& str) = 0;
    virtual int calc(int a, double b) = 0;
};

class Target {
public:
    Target(Parent* parent) :
        parent_(parent)
    { }

    int doThis()
    {
        int v = parent_->getNum();
        parent_->setResult(v);
        while (v-- > 0) {
            parent_->print(to_string(v));
        }
        return parent_->getNum();
    }

    int doThat()
    {
        return parent_->calc(1, 2.2);
    }

private:
    Parent* parent_;
};


class MockParent : public Parent, public Mock<MockParent> {
public:
    //! MOCK_[CONST_]METHODx(方法名, 返回类型(参数类型列表));
    MOCK_CONST_METHOD0(getNum, int);    //! 由于 getNum() 是 const 成员函数，所以要使用 MOCK_CONST_METHODx
    MOCK_METHOD1(setResult, void, int, value);
    MOCK_METHOD1(print, void, const string&, str);
    MOCK_METHOD2(calc, int, int, a, double, b);
};


TEST(demo, 1) {
    MockParent p;
    Target t(&p);

    //! 设置 p.getNum() 方法的形为
    EXPECT_CALL(p, getNum())
        .Times(2)    //! 期望被调两次
        .WillOnce(Return(2))   //! 第一次返回值为2
        .WillOnce(Return(10)); //! 第二次返回值为10

    //! 设置 p.setResult(), 参数为2的调用形为
    EXPECT_CALL(p, setResult(2))
        .Times(1);

    EXPECT_CALL(p, print(_))  //! 表示任意的参数，其中 _ 就是 ::testing::_ ，如果冲突要注意
        .Times(2);

    EXPECT_EQ(t.doThis(), 10);
}

TEST(demo, 2) {
    MockParent p;
    Target t(&p);

    EXPECT_CALL(p, calc(1, 2.2))
        .Times(1)
        .WillOnce(Return(3));

    EXPECT_EQ(t.doThat(), 3);
}

int main() {
    UnitTest& unitTest = UnitTest::getInstance();
    return unitTest.runAllTests("test_report2.txt");
}



