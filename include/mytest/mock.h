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

// �޸� AtLeast ���������� TimesSpec
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
        std::deque<std::function<std::any()>> onceActions;  // ���ڴ洢��� WillOnce ��Ϊ
        std::function<std::any()> repeatedAction;  // ���� WillRepeatedly ����Ϊ
        std::function<std::any()> action;
        bool retireOnSaturation = false;
        int callCount = 0;  // ��¼ʵ�ʵ��ô���

        Expectation(int min, int max, Predicate pred)
            : minCount(min), maxCount(max), predicate(pred), action(nullptr), retireOnSaturation(false) {}
    };

    struct ExpectationBuilder {
        Mock<T>& mock;
        std::string methodName;
        std::shared_ptr<Expectation> expectation;

        ExpectationBuilder(Mock<T>& m, const std::string& name, std::shared_ptr<Expectation> exp)
            : mock(m), methodName(name), expectation(exp) {}   // ʹ�ô��ݵ� exp
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

        // ֧�ֶ�� WillOnce
        ExpectationBuilder& WillOnce(std::function<std::any()> act) {
            expectation->onceActions.push_back(act);

        /*    std::cout << "WillOnce: Expectation address: " << expectation.get() << std::endl;
            std::cout << "WillOnce: onceActions size: " << expectation->onceActions.size() << std::endl;*/

            return *this;
        }

        // WillRepeatedly ���ڶ����ظ�������Ϊ
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
        // ȥ��ĩβ�����ţ�����У�
        if (!methodKey.empty() && methodKey.back() == ')') {
            // �ҵ������ŵ�λ�ò��ü�����
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

    // �ǳ����汾���޸�״̬
    void recordCall(const std::string& methodName, const std::vector<std::any>& args = {}) {
        auto& expList = expectations[methodName];
        for (auto& exp : expList) {
            exp->callCount++;

            // ����� WillOnce ��Ϊ
            if (!exp->onceActions.empty()) {
                auto action = exp->onceActions.front();
                exp->onceActions.pop_front();  // ִ�к��Ƴ�
                exp->action = action;  // ��ִ�еĶ����洢Ϊ action
            }
            // ����ִ�� WillRepeatedly ��Ϊ
            else if (exp->repeatedAction) {
                exp->action = exp->repeatedAction;  // ���ظ������洢Ϊ action
            }

            // ִ�д洢�Ķ���
            if (exp->action) {
                exp->action();  // ʵ��ִ�ж���
            }

            // ����Ƿ���Ҫ���ݱ��������Ƴ�����
            if (exp->retireOnSaturation && exp->callCount > exp->maxCount) {
                expList.erase(std::remove(expList.begin(), expList.end(), exp), expList.end());
                break;
            }
        }
        callCounts[methodName]++;
    }

    // �����汾�����޸�״̬
    void recordCall(const std::string& methodName, const std::vector<std::any>& args = {}) const {
        // �������ѡ��򵥵ص��÷ǳ����汾
        // ���ߴ��������߼�
        // ������÷ǳ����汾��ע�⴫����ȷ�Ĳ���
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
            // ���û�������κ�����������Ĭ��ֵ
            if constexpr (std::is_same_v<R, void>) {
                return;
            }
            else {
                return R();  // ����Ĭ��ֵ
            }
        }

        auto exp = expList.front();
        std::any result;

        // ִ�� WillOnce �� WillRepeatedly ���õĶ�������ȡ���
        if (exp->action) {
            result = exp->action();  // ���ô洢�� action
        }
        else {
            // û������ WillOnce �� WillRepeatedly ʱ������Ĭ��ֵ
            if constexpr (std::is_same_v<R, void>) {
                return;
            }
            else {
                return R();  // ����Ĭ��ֵ
            }
        }

        // ����з���ֵ����鷵�������Ƿ�ƥ��
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

    //        // ����� WillOnce ��Ϊ
    //        if (!exp->onceActions.empty()) {
    //            auto action = exp->onceActions.front();
    //            exp->onceActions.pop_front();  // ִ�к��Ƴ�
    //            exp->action = action;  // ��ִ�еĶ����洢Ϊ action
    //        }
    //        // ����ִ�� WillRepeatedly ��Ϊ
    //        else if (exp->repeatedAction) {
    //            exp->action = exp->repeatedAction;  // ���ظ������洢Ϊ action
    //        }

    //        // ִ�д洢�Ķ���
    //        if (exp->action) {
    //            exp->action();  // ʵ��ִ�ж���
    //        }


    //        // ����Ƿ���Ҫ���ݱ��������Ƴ�����
    //        if (exp->retireOnSaturation && exp->callCount > exp->maxCount) {
    //            //throw std::runtime_error("Method call exceeded expected Times limit.");
    //            //return; // ֹͣ����ִ��
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
    //        // ���û�������κ�����������Ĭ��ֵ
    //        if constexpr (std::is_same_v<R, void>) {
    //            return;
    //        }
    //        else {
    //            return R();  // ����Ĭ��ֵ
    //        }
    //    }

    //    auto exp = expList.front();
    //    std::any result;

    //    // ִ�� WillOnce �� WillRepeatedly ���õĶ�������ȡ���
    //    if (exp->action) {
    //        result = exp->action();  // ���ô洢�� action
    //    }
    //    else {
    //        // û������ WillOnce �� WillRepeatedly ʱ������Ĭ��ֵ
    //        if constexpr (std::is_same_v<R, void>) {
    //            return;
    //        }
    //        else {
    //            return R();  // ����Ĭ��ֵ
    //        }
    //    }

    //    // ����з���ֵ����鷵�������Ƿ�ƥ��
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
