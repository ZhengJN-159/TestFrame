//////
////#include "../include/mytest.h"
////#include "../include/mytest/mock.h"
////#include <stdbool.h>
////#include <iostream>
////
////#include <map>
////#include <vector>
////#include <functional>
////#include <any>
////#include <deque>
////#include <iostream>
////#include <exception>
////#include <memory>
////#include <string>
////#include <limits>
////
//////接口
////class Turtle {
////public:
////	//控制turtle运动是否留下痕迹
////	virtual void PenUp() = 0;	//画笔抬起
////	virtual void PenDown() = 0;	//画笔落下
////	//控制turtle运动方向
////	virtual void Forward(int distance) = 0;	//前进
////	virtual void Turn(int degrees) = 0;		//转向
////	virtual void GoTo(int x, int y) = 0;	//前往(x,y)位置
////	//获取turtle当前位置
////	virtual int GetX() = 0;	//获取x的坐标
////	virtual int GetY() = 0;	//获取y的坐标
////
////public:
////	virtual ~Turtle() {}	//析构函数必须是virtual
////};
////
////class Painter
////{
////	Turtle* turtle;
////public:
////	Painter(Turtle* turtle)
////		: turtle(turtle) {}
////
////	bool DrawCircle(int, int, int) {
////		turtle->PenDown();
////		return true;
////	}
////
////	int DrawXandY() {
////		int x = turtle->GetX();
////		int y = turtle->GetY();
////		return x + y;
////	}
////
////};
////
////class MockTurtle :public Mock<MockTurtle>, public Turtle{
////public:
////	//MOCK_METHOD(PenUp, void);
////	//MOCK_METHOD(PenDown,void);
////	//MOCK_METHOD(Forward,void, int distance);
////	//MOCK_METHOD(Turn, void, int degrees);
////	//MOCK_METHOD(GoTo, void, int x, int y);
////	//MOCK_METHOD(GetX,int);
////	//MOCK_METHOD(GetY,int);
////	// 
////	MOCK_METHOD0(PenUp, void);
////	MOCK_METHOD0(PenDown, void);
////	MOCK_METHOD1(Forward, void, int, distance);
////	MOCK_METHOD1(Turn, void, int, degrees);
////	MOCK_METHOD2(GoTo, void, int, x, int, y);
////	MOCK_METHOD0(GetX, int);
////	MOCK_METHOD0(GetY, int);
////	// 
////	//MOCK_METHOD(PenUp, void);
////	//MOCK_METHOD(PenDown, void);
////	//MOCK_METHOD(Forward, void, int, distance);
////	//MOCK_METHOD(Turn, void, int, degrees);
////	//MOCK_METHOD(GoTo, void, int, x, int, y);
////	//MOCK_METHOD(GetX, int);
////	//MOCK_METHOD(GetY, int);
////};
////
//////class MockTurtle : public Mock<MockTurtle>, public Turtle {
//////public:
//////	MOCK_METHOD(PenUp, void, ());
//////	MOCK_METHOD(PenDown, void, ());
//////	MOCK_METHOD(Forward, void, (int distance));
//////	MOCK_METHOD(Turn, void, (int degrees));
//////	MOCK_METHOD(GoTo, void, (int x, int y));
//////	MOCK_METHOD(GetX, int, ());
//////	MOCK_METHOD(GetY, int, ());
//////};
////
////TEST(PainterTest, CanDrawSomething) {
////	MockTurtle turtle;               // #2
////	EXPECT_CALL(turtle, PenDown())   // #3
////		.Times(AtLeast(5));
////
////	Painter painter(&turtle);
////
////	EXPECT_TRUE(painter.DrawCircle(0, 0, 10));      // #4
////
////	EXPECT_CALL(turtle, GetX())		// #3
////		.WillOnce(Return(10));		//x返回10
////	EXPECT_CALL(turtle, GetY())		// #3
////		.WillOnce(Return(20));		//y返回20
////
////	//EXPECT_CALL(turtle, GetX())
////	//	.WillOnce(Return(10))
////	//	.WillOnce(Return(15));
////	//EXPECT_CALL(turtle, GetY())
////	//	.WillOnce(Return(20))// 第一次调用返回 20
////	//	.WillRepeatedly(Return(25));// 之后每次调用都返回 25
////
////	//std::cout << painter.DrawXandY() << std::endl; // 预期输出：GetX: 10, GetY: 20
////	//std::cout << painter.DrawXandY() << std::endl; // 预期输出：GetX: 15, GetY: 25
////
////	//EXPECT_CALL(turtle, GetX())
////	//	.Times(Exactly(3))
////	//	.WillOnce(Return(10))
////	//	.WillRepeatedly(Return(20))
////	//	.RetiresOnSaturation();
////
////	//std::cout << painter.DrawXandY() << std::endl; // 预期输出：GetX: 10, GetY: 20
////	//std::cout << painter.DrawXandY() << std::endl; // 预期输出：GetX: 15, GetY: 25
////	//std::cout << painter.DrawXandY() << std::endl; // 预期输出：GetX: 10, GetY: 20
////
////	EXPECT_EQ(30, painter.DrawXandY());	//#4
////}
//// 
////int main() {
////    UnitTest& unit_test = UnitTest::getInstance();
////    return unit_test.runAllTests();
////}
////
////
//////
//
//
//#include <string>
//#include "../include/mytest.h"
//using namespace std;
//
//class Parent {
//public:
//    virtual ~Parent() {}
//
//    virtual int getNum() const = 0;
//    virtual void setResult(int value) = 0;
//    virtual void print(const string& str) = 0;
//    virtual int calc(int a, double b) = 0;
//};
//
//class Target {
//public:
//    Target(Parent* parent) :
//        parent_(parent)
//    { }
//
//    int doThis()
//    {
//        int v = parent_->getNum();
//        parent_->setResult(v);
//        while (v-- > 0) {
//            parent_->print(to_string(v));
//        }
//        return parent_->getNum();
//    }
//
//    int doThat()
//    {
//        return parent_->calc(1, 2.2);
//    }
//
//private:
//    Parent* parent_;
//};
//
//
//class MockParent : public Parent , public Mock<MockParent> {
//public:
//    //! MOCK_[CONST_]METHODx(方法名, 返回类型(参数类型列表));
//    MOCK_CONST_METHOD0(getNum, int);    //! 由于 getNum() 是 const 成员函数，所以要使用 MOCK_CONST_METHODx
//    MOCK_METHOD1(setResult, void,int,value);
//    MOCK_METHOD1(print,void,const string&,str);
//    MOCK_METHOD2(calc, int,int,a,double,b);
//};
//
//
//TEST(demo, 1) {
//    MockParent p;
//    Target t(&p);
//
//    //! 设置 p.getNum() 方法的形为
//    EXPECT_CALL(p, getNum())
//        .Times(2)    //! 期望被调两次
//        .WillOnce(Return(2))   //! 第一次返回值为2
//        .WillOnce(Return(10)); //! 第二次返回值为10
//
//    //! 设置 p.setResult(), 参数为2的调用形为
//    EXPECT_CALL(p, setResult(2))
//        .Times(1);
//
//    EXPECT_CALL(p, print(_))  //! 表示任意的参数，其中 _ 就是 ::testing::_ ，如果冲突要注意
//        .Times(2);
//
//    EXPECT_EQ(t.doThis(), 10);
//}
//
//TEST(demo, 2) {
//    MockParent p;
//    Target t(&p);
//
//    EXPECT_CALL(p, calc(1, 2.2))
//        .Times(1)
//        .WillOnce(Return(3));
//
//    EXPECT_EQ(t.doThat(), 3);
//}
//
//int main() {
//    UnitTest& unit_test = UnitTest::getInstance();
//    return unit_test.runAllTests();
//}



///*
//* TEST(SampleTest, TrueIsTrue) {
//    ASSERT_TRUE(true);
//    return 0;
//}
//
//TEST(SampleTest, FalseIsFalse) {
//    ASSERT_FALSE(false);
//    return 0;
//}
//*/
//
//
//
//
//TEST(SampleTest, EqualInts) {
//    ASSERT_EQ(1, 1);
//    return;
//}
//
//TEST(SampleTest, NotEqualInts) {
//    ASSERT_NE(1, 2);
//    return;
//}
//
//TEST(SampleTest, LessThan) {
//    ASSERT_LT(1, 2);
//    return;
//
//TEST(SampleTest, LessThanOrEqual) {
//    ASSERT_LE(1, 1);
//    return;
//}
//
//TEST(SampleTest, GreaterThan) {
//    ASSERT_GT(2, 1);
//    return;
//}
//
//TEST(SampleTest, GreaterThanOrEqual) {
//    ASSERT_GE(1, 2);
//    return;
//}
//
//TEST(SampleTest, NearEqual) {
//    ASSERT_NEAR(1.0, 1.001, 0.01);
//    return;
//}
//
//TEST(SampleTest, ThrowsException) {
//    ASSERT_THROW(throw std::runtime_error("error"), std::runtime_error);
//    return;
//}
//
//TEST(SampleTest, NoThrowException) {
//    ASSERT_NO_THROW(int a = 1 + 1);
//    return;
//}
//
//TEST(SampleTest, ExpectTest) {
//    EXPECT_TRUE(true);
//    EXPECT_FALSE(false);
//    EXPECT_EQ(0, 1);
////    EXPECT_NE(1, 2);
//    return;
//}

//TEST(SampleTest, DurationLessThan) {
//    ASSERT_DURATION_LESS_THAN(100, {
//        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate some work
//        });
//    return;
//}
//
//// 使用测试夹具
//TEST_F(MyTestFixture, TestWithFixture) {
//    value = 42;
//    ASSERT_EQ(42, value);
//}
//
//


//
//










//TEST_SUITE(CalculatorTestSuite);
//
//TEST_IN_SUITE(CalculatorTestSuite, CalculatorTest, Addition) {
//    Calculator calculator;
//    ASSERT_EQ(5, calculator.add(2, 3));
//}
//
//TEST_IN_SUITE(CalculatorTestSuite, CalculatorTest, Subtraction) {
//    Calculator calculator;
//    ASSERT_EQ(-1, calculator.subtract(2, 3));
//}
//
//TEST_IN_SUITE(CalculatorTestSuite, CalculatorTest, Multiplication) {
//    Calculator calculator;
//    ASSERT_EQ(6, calculator.multiply(2, 3));
//}
//
//TEST_IN_SUITE(CalculatorTestSuite, CalculatorTest, Division) {
//    Calculator calculator;
//    ASSERT_NEAR(2.0, calculator.divide(4, 2), 0.001);
//}
//
//TEST_IN_SUITE(CalculatorTestSuite, CalculatorTest, DivisionByZero) {
//    Calculator calculator;
//    ASSERT_THROW(calculator.divide(4, 0), std::invalid_argument);
//}
//
// 模拟的类定义
//class Database {
//public:
//    bool Connect() { return true; }
//    bool Query(const std::string& query) { return query == "valid_query"; }
//};
//
//class User {
//public:
//    User(Database* db) : db_(db) {
//        if (!db_->Connect()) {
//            throw std::runtime_error("Failed to connect to database");
//        }
//    }
//    bool IsValidUser(const std::string& username) {
//        return db_->Query("valid_query");
//    }
//private:
//    Database* db_;
//};
//
//class Authentication {
//public:
//    Authentication(User* user) : user_(user) {}
//    bool Authenticate(const std::string& username, const std::string& password) {
//        return user_->IsValidUser(username) && password == "password";
//    }
//private:
//    User* user_;
//};
//
//class Application {
//public:
//    Application(Authentication* auth) : auth_(auth) {}
//    bool Login(const std::string& username, const std::string& password) {
//        return auth_->Authenticate(username, password);
//    }
//private:
//    Authentication* auth_;
//};
//
//// 测试夹具
//class MyComplexTestFixture : public TestFixtureTemplate<MyComplexTestFixture> {
//public:
//    void SetUp() override {
//        // 初始化所有相关类
//        db = new Database();
//        user = new User(db);
//        auth = new Authentication(user);
//        app = new Application(auth);
//    }
//
//    void TearDown() override {
//        // 释放所有资源
//        delete app;
//        delete auth;
//        delete user;
//        delete db;
//    }
//
//    Database* db;
//    User* user;
//    Authentication* auth;
//    Application* app;
//};
//
//// 测试Application的Login方法
//TEST_F(MyComplexTestFixture, LoginTest) {
//    EXPECT_TRUE(app->Login("valid_user", "password"));
//    EXPECT_FALSE(app->Login("valid_user", "wrong_password"));
//    EXPECT_FALSE(app->Login("invalid_user", "password"));
//}
//
//// 测试User的IsValidUser方法
//TEST_F(MyComplexTestFixture, IsValidUserTest) {
//    EXPECT_TRUE(user->IsValidUser("valid_user"));
//    EXPECT_FALSE(user->IsValidUser("invalid_user"));
//}
//
//// 测试Authentication的Authenticate方法
//TEST_F(MyComplexTestFixture, AuthenticateTest) {
//    EXPECT_TRUE(auth->Authenticate("valid_user", "password"));
//    EXPECT_FALSE(auth->Authenticate("valid_user", "wrong_password"));
//    EXPECT_FALSE(auth->Authenticate("invalid_user", "password"));
//}
//
//// 测试Database的Query方法
//TEST_F(MyComplexTestFixture, QueryTest) {
//    EXPECT_TRUE(db->Query("valid_query"));
//    EXPECT_FALSE(db->Query("invalid_query"));
//}
//
//int main() {
//    UnitTest& unit_test = UnitTest::getInstance();
//
//    ParameterizedTest_Equals_Params = { 1, 2, 3, 4, 5 }; // 设置测试参数
//    return unit_test.runAllTests();
//}
// 
// 