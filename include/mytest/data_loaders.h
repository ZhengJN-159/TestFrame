#ifndef MYTEST_DATA_LOADER_H
#define MYTEST_DATA_LOADER_H

#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>  // You need to include a JSON library like nlohmann/json

using json = nlohmann::json;

class DataLoader {
public:
    // 从JSON文件加载参数
 // 从JSON文件加载参数
    static std::vector<std::map<std::string, int>> loadJSON(const std::string& filePath) {
        std::ifstream file(filePath);
        nlohmann::json jsonData;
        file >> jsonData;

        std::vector<std::map<std::string, int>> params;
        for (const auto& item : jsonData) {
            std::map<std::string, int> paramSet;
            for (auto it = item.begin(); it != item.end(); ++it) {
                paramSet[it.key()] = it.value();
            }
            params.push_back(paramSet);
        }

        return params;
    }

    static std::vector<std::map<std::string, int>> loadCSV(const std::string& filePath) {
        std::vector<std::map<std::string, int>> data;
        std::ifstream file(filePath);
        std::string line;

        // 读取标题行
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<std::string> headers;
            std::string header;

            while (std::getline(ss, header, ',')) {
                headers.push_back(header);
            }

            // 读取数据行
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::map<std::string, int> row;

                for (const auto& header : headers) {
                    std::string value;
                    if (std::getline(ss, value, ',')) {
                        row[header] = std::stoi(value);
                    }
                }
                data.push_back(row);
            }
        }

        return data;
    }
    static std::vector<std::map<std::string, int>> loadXML(const std::string& filePath) {
        std::vector<std::map<std::string, int>> data;
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filePath.c_str());

        tinyxml2::XMLElement* root = doc.FirstChildElement("tests");
        if (root) {
            for (tinyxml2::XMLElement* testElement = root->FirstChildElement("test");
                testElement != nullptr;
                testElement = testElement->NextSiblingElement("test")) {

                std::map<std::string, int> row;
                for (tinyxml2::XMLElement* paramElement = testElement->FirstChildElement();
                    paramElement != nullptr;
                    paramElement = paramElement->NextSiblingElement()) {
                    const char* paramName = paramElement->Name();
                    int paramValue = paramElement->IntText();
                    row[paramName] = paramValue; // 动态添加参数
                }
                data.push_back(row);
            }
        }

        return data;
    }

    static std::vector<std::vector<int>> loadData(const std::string& file_path) {
        std::vector<std::vector<int>> data;
        std::ifstream input(file_path);

        if (!input) {
            throw std::runtime_error("Cannot open file: " + file_path);
        }

        json jsonData;
        try {
            input >> jsonData;
        }
        catch (const json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }


        for (const auto& item : jsonData) {
            std::vector<int> params;
            for (const auto& param : item) {
                params.push_back(param.get<int>());
            }
            data.push_back(params);
        }

        return data;
    }
};

#endif // MYTEST_DATA_LOADER_H
