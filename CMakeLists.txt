# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyTestFramework)

set(CMAKE_CXX_STANDARD 11)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 创建静态库
add_library(mytest STATIC ${SOURCES})

# 安装头文件和库文件
install(TARGETS mytest DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/mytest DESTINATION include)

