
# MyTestFramework

## 介绍

MyTestFramework 是一个轻量级的 C++ 单元测试框架。

## 特性

- **基本断言**：提供多种断言宏，支持布尔条件、相等、不等、比较、近似相等和异常处理等。
- **测试夹具**：支持复杂的测试夹具，以便在不同测试间共享设置和状态。
- **参数化测试**：通过 `TEST_P` 宏实现参数化测试，支持从外部文件加载测试数据（如 JSON、CSV、XML）。
- **模拟功能**：提供 `EXPECT_CALL`、`MOCK_METHOD` 等宏，支持灵活的模拟功能。
- **模块接口测试**：专注于模块之间接口的测试，确保模块间的正确交互。
- **测试报告**：自动生成测试报告，方便查看测试结果。

## 安装

### 前提条件

- C++ 编译器（例如，GCC、Clang、MSVC）
- 用于构建配置的 CMake

### 构建框架

1. 克隆仓库：

2. 创建构建目录并进入该目录：
   ```bash
   mkdir build
   cd build
   ```

3. 运行 CMake 配置项目：
   ```bash
   cmake ..
   ```

4. 构建项目：
   ```bash
   make
   ```

## 使用（具体示例可见"tests\sample_test.cpp"）

### 编写简单测试用例

在 `tests` 目录中创建测试文件。每个测试文件应包含必要的头文件并定义测试用例。示例如下：

```cpp
#include "mytest.h"

TEST(SampleTest, BasicAssertions) {
    ASSERT_EQ(1 + 1, 2);
    ASSERT_TRUE(true);
}

```
ASSERT_EQ、ASSERT_TRUE 用于验证相等性、真假性。
每个测试用例以 TEST 宏定义。



### 参数化测试

要编写参数化测试，请定义测试参数并实现测试用例，如下所示：

```cpp
#include "mytest.h"

using ParamType = std::map<std::string, int>;

TEST_P(MyTest, ComplexEquals, ParamType, "test_data.json", DataLoader::loadJSON) {
    int param1 = param.at("param1");
    int param2 = param.at("param2");
    int param3 = param.at("param3");
    EXPECT_EQ(param1 + param2, param3);  // 验证 param1 + param2 是否等于 param3
}

```
TEST_P 用于定义参数化测试。
通过 DataLoader 从 JSON 文件（CSV、XML)加载数据。
测试逻辑检查数据中的 param1 和 param2 的和是否等于 param3。



### 测试夹具

使用测试夹具来设置和拆除公共的测试资源：

```cpp
#include "mytest.h"

class MyComplexFixture : public TestFixtureTemplate<MyComplexFixture> {
public:
    void SetUp() override {
        calculator = new Calculate();   // 初始化 Calculate 对象
        statistics = new Statistics();   // 初始化 Statistics 对象
    }

    void TearDown() override {
        delete calculator;   // 清理资源
        delete statistics;
    }

    Calculate* calculator;
    Statistics* statistics;
};

TEST_F(MyComplexFixture, CalculatorAddTest) {
    EXPECT_EQ(7, calculator->Add(3, 4));  // 验证加法结果
}

```
MyComplexFixture 是一个测试夹具，提供了测试所需的共享资源。
SetUp 和 TearDown 方法用于初始化和清理资源。
TEST_F 宏用于在夹具中定义测试用例。

### 测试套件

使用测试夹具来设置和拆除公共的测试资源：

```cpp
#include "mytest.h"

TEST_SUITE(SampleSuite);

TEST_IN_SUITE(SampleSuite, SampleTest, TrueIsTrue) {
    ASSERT_TRUE(true);  // 验证条件为真
}

```
TEST_SUITE 定义了一个测试套件。
TEST_IN_SUITE 用于在指定套件中定义测试用例。


### 接口测试

使用测试夹具来设置和拆除公共的测试资源：

```cpp
#include "mytest.h"

class MockParent : public Parent, public Mock<MockParent> {
public:
    MOCK_CONST_METHOD0(getNum, int);
    MOCK_METHOD1(setResult, void, int, value);
};

TEST(demo, 1) {
    MockParent p;
    Target t(&p);
    EXPECT_CALL(p, getNum())
        .Times(2)
        .WillOnce(Return(2))
        .WillOnce(Return(10));

    EXPECT_EQ(t.doThis(), 10);  // 验证 doThis() 方法的返回值
}

```
MockParent 是一个模拟类，用于测试与接口的交互。
EXPECT_CALL 用于设置期望行为和返回值。

### 运行测试

编写完测试后，使用以下代码在 main 函数中运行所有测试，可输出报告：

```cpp
#include "mytest.h"

int main() {
    return UnitTest::getInstance().runAllTests("test_report.txt");
}
```
运行所有测试并将结果输出到指定的报告文件中。


### 断言详解

以下是 `assert.h`文件中每个断言的详细介绍：

| 断言宏 | 描述 |
| --- | --- |
| `ASSERT_TRUE(condition)` | 断言条件为 true。如果条件为 false，则测试失败。 |
| `ASSERT_FALSE(condition)` | 断言条件为 false。如果条件为 true，则测试失败。 |
| `ASSERT_EQ(val1, val2)` | 断言 `val1` 与 `val2` 相等。如果不相等，则测试失败。 |
| `ASSERT_NE(val1, val2)` | 断言 `val1` 与 `val2` 不相等。如果相等，则测试失败。 |
| `ASSERT_LT(val1, val2)` | 断言 `val1` 小于 `val2`。如果不小于，则测试失败。 |
| `ASSERT_LE(val1, val2)` | 断言 `val1` 小于或等于 `val2`。如果大于，则测试失败。 |
| `ASSERT_GT(val1, val2)` | 断言 `val1` 大于 `val2`。如果不大于，则测试失败。 |
| `ASSERT_GE(val1, val2)` | 断言 `val1` 大于或等于 `val2`。如果小于，则测试失败。 |
| `ASSERT_NEAR(val1, val2, abs_error)` | 断言 `val1` 与 `val2` 的差的绝对值在 `abs_error` 之内。如果超出范围，则测试失败。 |
| `ASSERT_THROW(statement, exception_type)` | 断言 `statement` 抛出 `exception_type` 类型的异常。如果未抛出或抛出其他类型的异常，则测试失败。 |
| `ASSERT_NO_THROW(statement)` | 断言 `statement` 不抛出任何异常。如果抛出异常，则测试失败。 |
| `ASSERT_ANY_THROW(statement)` | 断言 `statement` 抛出任何类型的异常。如果未抛出异常，则测试失败。 |
| `EXPECT_TRUE(condition)` | 期望条件为 true。如果条件为 false，则记录失败。 |
| `EXPECT_FALSE(condition)` | 期望条件为 false。如果条件为 true，则记录失败。 |
| `EXPECT_EQ(val1, val2)` | 期望 `val1` 与 `val2` 相等。如果不相等，则记录失败。 |
| `EXPECT_NE(val1, val2)` | 期望 `val1` 与 `val2` 不相等。如果相等，则记录失败。 |
| `EXPECT_LT(val1, val2)` | 期望 `val1` 小于 `val2`。如果不小于，则记录失败。 |
| `EXPECT_LE(val1, val2)` | 期望 `val1` 小于或等于 `val2`。如果大于，则记录失败。 |
| `EXPECT_GT(val1, val2)` | 期望 `val1` 大于 `val2`。如果不大于，则记录失败。 |
| `EXPECT_GE(val1, val2)` | 期望 `val1` 大于或等于 `val2`。如果小于，则记录失败。 |
| `EXPECT_NEAR(val1, val2, abs_error)` | 期望 `val1` 与 `val2` 的差的绝对值在 `abs_error` 之内。如果超出范围，则记录失败。 |
| `EXPECT_THROW(statement, exception_type)` | 期望 `statement` 抛出 `exception_type` 类型的异常。如果未抛出或抛出其他类型的异常，则记录失败。 |
| `EXPECT_NO_THROW(statement)` | 期望 `statement` 不抛出任何异常。如果抛出异常，则记录失败。 |
| `EXPECT_ANY_THROW(statement)` | 期望 `statement` 抛出任何类型的异常。如果未抛出异常，则记录失败。 |

