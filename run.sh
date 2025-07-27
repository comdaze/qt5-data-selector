#!/bin/bash

# Qt5 数据筛选器运行脚本

echo "Qt5 数据筛选器启动脚本"
echo "===================="

# 检查可执行文件是否存在
if [ -f "bin/Qt5DataSelector" ]; then
    echo "使用qmake构建的版本..."
    cd bin && ./Qt5DataSelector
elif [ -f "build/bin/Qt5DataSelector" ]; then
    echo "使用CMake构建的版本..."
    cd build/bin && ./Qt5DataSelector
else
    echo "错误: 找不到可执行文件"
    echo "请先运行构建脚本: ./build.sh"
    exit 1
fi