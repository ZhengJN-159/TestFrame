#ifndef MYTEST_FIXTURE_TEMPLATE_H
#define MYTEST_FIXTURE_TEMPLATE_H

template<typename T>
class TestFixtureTemplate : public TestFixture {
public:
    void SetUp() override {
        static_cast<T*>(this)->SetUp();
    }

    void TearDown() override {
        static_cast<T*>(this)->TearDown();
    }
};

#endif // MYTEST_FIXTURE_TEMPLATE_H