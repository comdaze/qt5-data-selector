#!/bin/bash

# Qt5 数据筛选器构建脚本

set -e

echo "Qt5 数据筛选器构建脚本"
echo "======================"

# 检查构建方式参数
BUILD_TYPE="cmake"
if [ "$1" = "qmake" ]; then
    BUILD_TYPE="qmake"
elif [ "$1" = "cmake" ] || [ -z "$1" ]; then
    BUILD_TYPE="cmake"
else
    echo "用法: $0 [cmake|qmake]"
    echo "  cmake  - 使用CMake构建 (默认)"
    echo "  qmake  - 使用qmake构建"
    exit 1
fi

echo "构建方式: $BUILD_TYPE"

if [ "$BUILD_TYPE" = "cmake" ]; then
    echo "使用CMake构建..."
    
    # 创建构建目录
    if [ ! -d "build" ]; then
        mkdir build
    fi
    
    cd build
    
    # 配置项目
    echo "配置项目..."
    cmake ..
    
    # 编译项目
    echo "编译项目..."
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    echo "构建完成！"
    echo "可执行文件位置: build/bin/Qt5DataSelector"
    echo "运行命令: cd build && ./bin/Qt5DataSelector"
    
elif [ "$BUILD_TYPE" = "qmake" ]; then
    echo "使用qmake构建..."
    
    # 清理之前的构建文件
    if [ -f "Makefile" ]; then
        make clean 2>/dev/null || true
        rm -f Makefile
    fi
    
    # 生成Makefile
    echo "生成Makefile..."
    qmake qt5-data-selector.pro
    
    # 编译项目
    echo "编译项目..."
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    echo "构建完成！"
    echo "可执行文件位置: bin/Qt5DataSelector"
    echo "运行命令: ./bin/Qt5DataSelector"
fi

echo ""
echo "构建成功完成！"