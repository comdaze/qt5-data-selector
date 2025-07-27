#!/bin/bash

# Qt5 数据筛选器依赖检查脚本

echo "Qt5 数据筛选器依赖检查"
echo "===================="

# 检查Qt5
echo "检查Qt5..."
if command -v qmake >/dev/null 2>&1; then
    QT_VERSION=$(qmake -query QT_VERSION)
    echo "✓ Qt版本: $QT_VERSION"
    
    # 检查Qt5模块
    QT_INSTALL_LIBS=$(qmake -query QT_INSTALL_LIBS)
    if [ -d "$QT_INSTALL_LIBS/QtCore.framework" ] || [ -f "$QT_INSTALL_LIBS/libQt5Core.so" ] || [ -f "$QT_INSTALL_LIBS/Qt5Core.lib" ]; then
        echo "✓ Qt5Core 可用"
    else
        echo "✗ Qt5Core 不可用"
    fi
    
    if [ -d "$QT_INSTALL_LIBS/QtWidgets.framework" ] || [ -f "$QT_INSTALL_LIBS/libQt5Widgets.so" ] || [ -f "$QT_INSTALL_LIBS/Qt5Widgets.lib" ]; then
        echo "✓ Qt5Widgets 可用"
    else
        echo "✗ Qt5Widgets 不可用"
    fi
else
    echo "✗ qmake 未找到，请安装Qt5开发包"
fi

# 检查CMake
echo ""
echo "检查CMake..."
if command -v cmake >/dev/null 2>&1; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    echo "✓ CMake版本: $CMAKE_VERSION"
    
    # 检查版本是否满足要求
    CMAKE_MAJOR=$(echo $CMAKE_VERSION | cut -d'.' -f1)
    CMAKE_MINOR=$(echo $CMAKE_VERSION | cut -d'.' -f2)
    
    if [ "$CMAKE_MAJOR" -gt 3 ] || ([ "$CMAKE_MAJOR" -eq 3 ] && [ "$CMAKE_MINOR" -ge 16 ]); then
        echo "✓ CMake版本满足要求 (>=3.16)"
    else
        echo "⚠ CMake版本过低，建议升级到3.16或更高版本"
    fi
else
    echo "✗ cmake 未找到，请安装CMake"
fi

# 检查编译器
echo ""
echo "检查编译器..."

# 检查GCC
if command -v gcc >/dev/null 2>&1; then
    GCC_VERSION=$(gcc --version | head -n1)
    echo "✓ GCC: $GCC_VERSION"
fi

# 检查Clang
if command -v clang >/dev/null 2>&1; then
    CLANG_VERSION=$(clang --version | head -n1)
    echo "✓ Clang: $CLANG_VERSION"
fi

# 检查G++
if command -v g++ >/dev/null 2>&1; then
    GPP_VERSION=$(g++ --version | head -n1)
    echo "✓ G++: $GPP_VERSION"
fi

# 检查Make
echo ""
echo "检查构建工具..."
if command -v make >/dev/null 2>&1; then
    MAKE_VERSION=$(make --version | head -n1)
    echo "✓ Make: $MAKE_VERSION"
else
    echo "✗ make 未找到"
fi

# 检查Ninja (可选)
if command -v ninja >/dev/null 2>&1; then
    NINJA_VERSION=$(ninja --version)
    echo "✓ Ninja: $NINJA_VERSION"
fi

# 系统信息
echo ""
echo "系统信息:"
echo "操作系统: $(uname -s)"
echo "架构: $(uname -m)"
if [ "$(uname -s)" = "Darwin" ]; then
    echo "macOS版本: $(sw_vers -productVersion)"
elif [ "$(uname -s)" = "Linux" ]; then
    if [ -f /etc/os-release ]; then
        echo "Linux发行版: $(grep PRETTY_NAME /etc/os-release | cut -d'"' -f2)"
    fi
fi

echo ""
echo "依赖检查完成！"