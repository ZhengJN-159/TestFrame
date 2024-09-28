cmake_minimum_required(VERSION 3.10)
project(MyTestFramework)

set(CMAKE_CXX_STANDARD 11)

# vcpkg 的工具链文件
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# 查找 nlohmann-json 和 tinyxml2 库（通过 vcpkg 安装的库）
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 创建静态库 mytest
add_library(mytest STATIC ${SOURCES})

# 链接 nlohmann-json 和 tinyxml2 库
target_link_libraries(mytest PRIVATE nlohmann_json::nlohmann_json tinyxml2)

# 安装头文件和库文件
install(TARGETS mytest DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/mytest DESTINATION include)

# 如果有测试文件，则添加测试目标
enable_testing()
add_executable(MyTestFramework_tests tests/sample_test2.cpp)
target_link_libraries(MyTestFramework_tests PRIVATE mytest nlohmann_json::nlohmann_json tinyxml2)

add_test(NAME MyTestFramework_tests COMMAND MyTestFramework_tests)
