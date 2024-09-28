#include <string>
#include "../include/mytest.h"
#include "../include/mytest/database_fixture.h"
using namespace std;

//1 ��ʹ��
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

//2 ��������
using ParamType = std::map<std::string, int>;
TEST_P(MyTest, ComplexEquals, ParamType, "test_data.json", DataLoader::loadJSON) {
    int param1 = param.at("param1");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    EXPECT_EQ(param1 + param2, param3)
    // �����߼������ param1 + param2 �Ƿ���� param3
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}

// ʹ�� CSV ���ݽ��в���
TEST_P(MyTest, ComplexEquals_CSV, ParamType, "test_data.csv", DataLoader::loadCSV) {
    int param1 = param.at("param1");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    // �����߼�
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}

// ʹ�� XML ���ݽ��в���
TEST_P(MyTest, ComplexEquals_XML, ParamType, "test_data.xml", DataLoader::loadXML) {
    int param1 = param.at("paramA");
    int param2 = param.at("param2");
    int param3 = param.at("param3");

    // �����߼�
    if (param1 + param2 != param3) {
        std::cout << "Test failed: " << param1 << " + " << param2 << " != " << param3 << std::endl;
    }
    else {
        std::cout << "Test passed: " << param1 << " + " << param2 << " == " << param3 << std::endl;
    }
}




//3 ���Լо�
// �����Ե���
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

// ���Լо�
class MyComplexFixture : public TestFixtureTemplate<MyComplexFixture>{
public:
    void SetUp() override {
        // ��ʼ��Calculator��Statistics����
        calculator = new Calculate();
        statistics = new Statistics();
    }

    void TearDown() override {
        // �ͷ���Դ
        delete calculator;
        delete statistics;
    }

    Calculate* calculator;
    Statistics* statistics;
};

// ����Calculator��Add����
TEST_F(MyComplexFixture, CalculatorAddTest) {
    EXPECT_EQ(7, calculator->Add(3, 4));
    EXPECT_EQ(calculator->Add(-1, 5), 4);
}

// ����Calculator��Divide����
TEST_F(MyComplexFixture, CalculatorDivideTest) {
    EXPECT_DOUBLE_EQ(calculator->Divide(10, 2), 5.0);
    EXPECT_THROW(calculator->Divide(1, 0), std::invalid_argument);
}

// ����Statistics��Mean����
TEST_F(MyComplexFixture, StatisticsMeanTest) {
    std::vector<int> data = { 1, 2, 3, 4, 5 };
    EXPECT_DOUBLE_EQ(statistics->Mean(data), 3.0);
    EXPECT_THROW(statistics->Mean({}), std::invalid_argument);
}

// ����Statistics��StandardDeviation����
TEST_F(MyComplexFixture, StatisticsStandardDeviationTest) {
    std::vector<int> data = { 1, 2, 3, 4, 5 };
    EXPECT_NEAR(statistics->StandardDeviation(data), 1.41421, 1e-5);
    EXPECT_THROW(statistics->StandardDeviation({}), std::invalid_argument);
}

// ����Calculator��Statistics�Ľ���
TEST_F(MyComplexFixture, CalculatorAndStatisticsInteractionTest) {
    std::vector<int> data = { calculator->Add(1, 2), calculator->Add(3, 4), calculator->Add(5, 6) };
    EXPECT_DOUBLE_EQ(statistics->Mean(data), 7.0);
    EXPECT_NEAR(statistics->StandardDeviation(data), 2.44949, 1e-5);
}




//4 �����׼�
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



//5 �ӿڲ���
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
    //! MOCK_[CONST_]METHODx(������, ��������(���������б�));
    MOCK_CONST_METHOD0(getNum, int);    //! ���� getNum() �� const ��Ա����������Ҫʹ�� MOCK_CONST_METHODx
    MOCK_METHOD1(setResult, void, int, value);
    MOCK_METHOD1(print, void, const string&, str);
    MOCK_METHOD2(calc, int, int, a, double, b);
};


TEST(demo, 1) {
    MockParent p;
    Target t(&p);

    //! ���� p.getNum() ��������Ϊ
    EXPECT_CALL(p, getNum())
        .Times(2)    //! ������������
        .WillOnce(Return(2))   //! ��һ�η���ֵΪ2
        .WillOnce(Return(10)); //! �ڶ��η���ֵΪ10

    //! ���� p.setResult(), ����Ϊ2�ĵ�����Ϊ
    EXPECT_CALL(p, setResult(2))
        .Times(1);

    EXPECT_CALL(p, print(_))  //! ��ʾ����Ĳ��������� _ ���� ::testing::_ �������ͻҪע��
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



