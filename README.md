
# MyTestFramework

## 介绍

MyTestFramework 是一个轻量级的 C++ 单元测试框架。

## 特性

- **断言**：提供多种断言宏，用于布尔条件、相等、不相等、比较、近似相等和异常处理。
- **参数化测试**：支持参数化测试。
- **测试夹具**：支持复杂的测试夹具，用于设置和拆除测试环境。

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

### 编写测试

在 `tests` 目录中创建测试文件。每个测试文件应包含必要的头文件并定义测试用例。示例如下：

```cpp
#include "mytest.h"

TEST(SampleTest, BasicAssertions) {
    ASSERT_EQ(1 + 1, 2);
    ASSERT_TRUE(true);
}
```

### 参数化测试

要编写参数化测试，请定义测试参数并实现测试用例，如下所示：

```cpp
#include "mytest.h"

TEST_P(MyTest, MyParameterizedTest) {
    int param = GetParam();
    ASSERT_TRUE(param % 2 == 0);
}

```

### 测试夹具

使用测试夹具来设置和拆除公共的测试资源：

```cpp
#include "mytest.h"

class MyFixture : public TestFixture {
protected:
    void SetUp() override {
        // 设置代码
    }

    void TearDown() override {
        // 清理代码
    }
};

TEST_F(MyFixture, TestName) {
    ASSERT_TRUE(true);
}
```

### 运行测试

编写完测试后，使用以下代码在 main 函数中运行所有测试：

```cpp
#include "mytest.h"

int main() {
    return UnitTest::getInstance().runAllTests();
}

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

