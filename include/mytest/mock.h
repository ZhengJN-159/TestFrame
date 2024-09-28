// mock.h
#pragma once
#define NOMINMAX
#ifdef max
#   undef max
#endif

#include <limits>
#include <map>
#include <vector>
#include <functional>
#include <any>
#include <deque>
#include <iostream>
#include <exception>
#include <memory>
#include <string>

enum class TimesCondition {
    Exactly,
    AtLeast,
    AtMost
};

struct TimesSpec {
    TimesCondition condition;
    int count;

    TimesSpec(TimesCondition cond, int c) : condition(cond), count(c) {}
};

// 修改 AtLeast 函数来返回 TimesSpec
inline TimesSpec AtLeast(int count) {
    return TimesSpec(TimesCondition::AtLeast, count);
}

inline TimesSpec AtMost(int count) {
    return TimesSpec(TimesCondition::AtMost, count);
}

inline TimesSpec Exactly(int count) {
    return TimesSpec(TimesCondition::Exactly, count);
}

template <typename R>
std::function<std::any()> Return(R value) {
    return [value]() -> std::any {
        return std::any(value);
        };
}

template <typename T>
class Mock {
public:
    using Predicate = std::function<bool(const std::vector<std::any>&)>;

    struct Expectation {
        int minCount;
        int maxCount;
        Predicate predicate;
        std::deque<std::function<std::any()>> onceActions;  // 用于存储多次 WillOnce 行为
        std::function<std::any()> repeatedAction;  // 用于 WillRepeatedly 的行为
        std::function<std::any()> action;
        bool retireOnSaturation = false;
        int callCount = 0;  // 记录实际调用次数

        Expectation(int min, int max, Predicate pred)
            : minCount(min), maxCount(max), predicate(pred), action(nullptr), retireOnSaturation(false) {}
    };

    struct ExpectationBuilder {
        Mock<T>& mock;
        std::string methodName;
        std::shared_ptr<Expectation> expectation;

        ExpectationBuilder(Mock<T>& m, const std::string& name, std::shared_ptr<Expectation> exp)
            : mock(m), methodName(name), expectation(exp) {}   // 使用传递的 exp
        // Times for exact number of calls


        ExpectationBuilder& Times(const TimesSpec& spec) {
            int count = spec.count;
            switch (spec.condition) {
            case TimesCondition::Exactly:
                expectation->minCount = count;
                expectation->maxCount = count;
                break;
            case TimesCondition::AtLeast:
                expectation->minCount = count;
                expectation->maxCount = std::numeric_limits<int>::max();
                break;
            case TimesCondition::AtMost:
                expectation->minCount = 0;
                expectation->maxCount = count;
                break;
            default:
                throw std::runtime_error("Unsupported Times condition");
            }
            return *this;
        }

        ExpectationBuilder& Times(int count) {
            expectation->minCount = count;
            expectation->maxCount = count;
            return *this;
        }

        // 支持多次 WillOnce
        ExpectationBuilder& WillOnce(std::function<std::any()> act) {
            expectation->onceActions.push_back(act);

        /*    std::cout << "WillOnce: Expectation address: " << expectation.get() << std::endl;
            std::cout << "WillOnce: onceActions size: " << expectation->onceActions.size() << std::endl;*/

            return *this;
        }

        // WillRepeatedly 用于定义重复调用行为
        ExpectationBuilder& WillRepeatedly(std::function<std::any()> act) {
            expectation->repeatedAction = act;
            return *this;
        }

        ExpectationBuilder& RetiresOnSaturation() {
            expectation->retireOnSaturation = true;
            return *this;
        }

        // Implement InSequence, After, etc., as needed.
    };

    //ExpectationBuilder expect_call(const std::string& methodName) {
    //    auto exp = std::make_shared<Expectation>(1, 1, nullptr);  
    //    expectations[methodName].push_back(exp);
    //    callOrder.push_back(methodName);
    //    return ExpectationBuilder(*this, methodName);
    //}
    ExpectationBuilder expect_call(const std::string& methodName) {
        std::string methodKey = methodName;
        // 去除末尾的括号（如果有）
        if (!methodKey.empty() && methodKey.back() == ')') {
            // 找到左括号的位置并裁剪括号
            size_t pos = methodKey.find('(');
            if (pos != std::string::npos) {
                methodKey = methodKey.substr(0, pos);
            }
        }

        auto exp = std::make_shared<Expectation>(1, 1, nullptr);
        expectations[methodKey].push_back(exp);
        callOrder.push_back(methodKey);
        return ExpectationBuilder(*this, methodKey, exp);
    }

    // 非常量版本，修改状态
    void recordCall(const std::string& methodName, const std::vector<std::any>& args = {}) {
        auto& expList = expectations[methodName];
        for (auto& exp : expList) {
            exp->callCount++;

            // 如果有 WillOnce 行为
            if (!exp->onceActions.empty()) {
                auto action = exp->onceActions.front();
                exp->onceActions.pop_front();  // 执行后移除
                exp->action = action;  // 将执行的动作存储为 action
            }
            // 否则执行 WillRepeatedly 行为
            else if (exp->repeatedAction) {
                exp->action = exp->repeatedAction;  // 将重复动作存储为 action
            }

            // 执行存储的动作
            if (exp->action) {
                exp->action();  // 实际执行动作
            }

            // 检查是否需要根据饱和条件移除期望
            if (exp->retireOnSaturation && exp->callCount > exp->maxCount) {
                expList.erase(std::remove(expList.begin(), expList.end(), exp), expList.end());
                break;
            }
        }
        callCounts[methodName]++;
    }

    // 常量版本，不修改状态
    void recordCall(const std::string& methodName, const std::vector<std::any>& args = {}) const {
        // 这里可以选择简单地调用非常量版本
        // 或者处理其他逻辑
        // 如果调用非常量版本，注意传递正确的参数
        const_cast<Mock*>(this)->recordCall(methodName, args);
    }

    template <typename R>
    R get_return_value(const std::string& methodName) {
        return const_cast<Mock*>(this)->get_return_value_impl<R>(methodName);
    }

    template <typename R>
    R get_return_value_impl(const std::string& methodName) const {
        auto& expList = expectations.at(methodName);

        if (expList.empty()) {
            // 如果没有设置任何期望，返回默认值
            if constexpr (std::is_same_v<R, void>) {
                return;
            }
            else {
                return R();  // 返回默认值
            }
        }

        auto exp = expList.front();
        std::any result;

        // 执行 WillOnce 或 WillRepeatedly 设置的动作并获取结果
        if (exp->action) {
            result = exp->action();  // 调用存储的 action
        }
        else {
            // 没有设置 WillOnce 或 WillRepeatedly 时，返回默认值
            if constexpr (std::is_same_v<R, void>) {
                return;
            }
            else {
                return R();  // 返回默认值
            }
        }

        // 如果有返回值，检查返回类型是否匹配
        if constexpr (!std::is_same_v<R, void>) {
            if (result.type() != typeid(R)) {
                throw std::runtime_error("Return type mismatch for method: " + methodName);
            }
            return std::any_cast<R>(result);
        }
    }
    //void recordCall(const std::string& methodName, const std::vector<std::any>& args = {}) {
    //    auto& expList = expectations[methodName];
    //    for (auto& exp : expList) {
    //        exp->callCount++;
    //   /*     std::cout << "recordCall: Expectation address: " << exp.get() << std::endl;
    //        std::cout << "recordCall: onceActions size: " << exp->onceActions.size() << std::endl;*/

    //        // 如果有 WillOnce 行为
    //        if (!exp->onceActions.empty()) {
    //            auto action = exp->onceActions.front();
    //            exp->onceActions.pop_front();  // 执行后移除
    //            exp->action = action;  // 将执行的动作存储为 action
    //        }
    //        // 否则执行 WillRepeatedly 行为
    //        else if (exp->repeatedAction) {
    //            exp->action = exp->repeatedAction;  // 将重复动作存储为 action
    //        }

    //        // 执行存储的动作
    //        if (exp->action) {
    //            exp->action();  // 实际执行动作
    //        }


    //        // 检查是否需要根据饱和条件移除期望
    //        if (exp->retireOnSaturation && exp->callCount > exp->maxCount) {
    //            //throw std::runtime_error("Method call exceeded expected Times limit.");
    //            //return; // 停止后续执行
    //            expList.erase(std::remove(expList.begin(), expList.end(), exp), expList.end());
    //            break;
    //        }
    //    }
    //    callCounts[methodName]++;
    //}

    //template <typename R>
    //R get_return_value(const std::string& methodName){
    //    auto& expList = expectations[methodName];

    //    if (expList.empty()) {
    //        // 如果没有设置任何期望，返回默认值
    //        if constexpr (std::is_same_v<R, void>) {
    //            return;
    //        }
    //        else {
    //            return R();  // 返回默认值
    //        }
    //    }

    //    auto exp = expList.front();
    //    std::any result;

    //    // 执行 WillOnce 或 WillRepeatedly 设置的动作并获取结果
    //    if (exp->action) {
    //        result = exp->action();  // 调用存储的 action
    //    }
    //    else {
    //        // 没有设置 WillOnce 或 WillRepeatedly 时，返回默认值
    //        if constexpr (std::is_same_v<R, void>) {
    //            return;
    //        }
    //        else {
    //            return R();  // 返回默认值
    //        }
    //    }

    //    // 如果有返回值，检查返回类型是否匹配
    //    if constexpr (!std::is_same_v<R, void>) {
    //        if (result.type() != typeid(R)) {
    //            throw std::runtime_error("Return type mismatch for method: " + methodName);
    //        }
    //        return std::any_cast<R>(result);
    //    }
    //}

    void verify() const {
        // Verification code omitted for brevity
        for (const auto& entry : callCounts) {
            std::cout << "Method " << entry.first << " called " << entry.second << " times." << std::endl;
        }
    }

protected:
    std::map<std::string, std::vector<std::shared_ptr<Expectation>>> expectations;
    std::map<std::string, int> callCounts;
    std::deque<std::string> callOrder;
};
