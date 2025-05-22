cmake_minimum_required(VERSION 3.0.0)
project(DSAP112_FinalProject VERSION 0.1.0 LANGUAGES C CXX)

# 设置SFML的根目录路径
set(SFML_ROOT "/opt/homebrew/opt/sfml")
# 找到SFML库
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

# 定义GUI目标
add_executable(GUI GUI.cpp)
target_compile_features(GUI PRIVATE cxx_std_17)
# 链接SFML库到GUI目标
target_link_libraries(GUI PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

# 定义PDOGS目标
add_executable(PDOGS PDOGS.cpp)
target_compile_features(PDOGS PRIVATE cxx_std_17)

# 设置项目名称和版本
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# 包含CPack模块
include(CPack)
