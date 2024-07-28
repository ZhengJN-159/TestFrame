#ifndef MYTEST_DATABASE_FIXTURE_H
#define MYTEST_DATABASE_FIXTURE_H

#include "fixture_template.h"
#include <string>
#include <vector>
#include <iostream>

// 模拟数据库连接类
class MockDatabase {
public:
    void connect() {
        std::cout << "Database connected" << std::endl;
        connected = true;
    }

    void disconnect() {
        std::cout << "Database disconnected" << std::endl;
        connected = false;
    }

    void executeQuery(const std::string& query) {
        if (connected) {
            std::cout << "Executing query: " << query << std::endl;
            queries.push_back(query);
        }
        else {
            std::cerr << "Failed to execute query. No connection." << std::endl;
        }
    }

    bool isConnected() const {
        return connected;
    }

    const std::vector<std::string>& getQueries() const {
        return queries;
    }

private:
    bool connected = false;
    std::vector<std::string> queries;
};

template<typename T>
class DatabaseFixtureTemplate : public TestFixtureTemplate<T> {
public:
    void SetUp() override {
        db.connect();
        static_cast<T*>(this)->SetUpDatabase();
    }

    void TearDown() override {
        static_cast<T*>(this)->TearDownDatabase();
        db.disconnect();
    }

protected:
    MockDatabase db;
};

#endif // MYTEST_DATABASE_FIXTURE_H
