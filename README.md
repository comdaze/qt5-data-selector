# Qt5 数据筛选器

一个基于Qt5的现代化数据筛选器应用程序，提供直观的用户界面用于筛选和管理各种类型的设备数据。

## 功能特性

- **时间筛选**: 支持多种时间颗粒度选择（15分钟、1小时、1天）
- **设备管理**: 按设备类型进行分层显示和选择
- **关键字搜索**: 实时搜索设备，支持模糊匹配
- **现代化界面**: 深蓝色主题，响应式布局设计
- **用户友好**: 直观的操作界面和良好的用户体验

## 系统要求

- Qt5.12 或更高版本
- C++11 或更高版本编译器
- CMake 3.16 或更高版本（使用CMake构建时）
- qmake（使用qmake构建时）

## 构建说明

### 使用CMake构建

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译项目
make

# 运行应用程序
./bin/Qt5DataSelector
```

### 使用qmake构建

```bash
# 生成Makefile
qmake qt5-data-selector.pro

# 编译项目
make

# 运行应用程序
./bin/Qt5DataSelector
```

## 项目结构

```
qt5-data-selector/
├── src/                    # 源代码文件
│   ├── main.cpp           # 应用程序入口点
│   ├── MainWindow.cpp     # 主窗口实现
│   ├── TimeWidget.cpp     # 时间控件实现
│   ├── DeviceWidget.cpp   # 设备控件实现
│   ├── CustomButton.cpp   # 自定义按钮实现
│   └── DeviceManager.cpp  # 设备管理器实现
├── include/               # 头文件
│   ├── MainWindow.h
│   ├── TimeWidget.h
│   ├── DeviceWidget.h
│   ├── CustomButton.h
│   ├── DeviceManager.h
│   └── DeviceInfo.h
├── resources/             # 资源文件
│   ├── styles.qss        # 样式表文件
│   └── app_icon.png      # 应用程序图标
├── tests/                # 测试文件
│   ├── unit/             # 单元测试
│   └── integration/      # 集成测试
├── CMakeLists.txt        # CMake构建配置
├── qt5-data-selector.pro # qmake项目文件
└── resources.qrc         # Qt资源文件
```

## 使用说明

### 时间筛选
1. 选择时间颗粒度（15分钟、1小时、1天）
2. 设置开始和结束时间
3. 使用快捷按钮选择常用时间范围（前一天、前三天）

### 设备选择
1. 在设备类型标签页中浏览不同类型的设备
2. 使用复选框选择或取消选择设备
3. 使用"全部"复选框进行批量选择
4. 查看已选择设备的数量统计

### 搜索功能
1. 在搜索框中输入关键字
2. 系统会实时过滤显示匹配的设备
3. 搜索结果保持设备的层级结构

## 开发说明

### 运行测试（可选）

如果需要运行测试，请取消注释CMakeLists.txt中的测试相关配置：

```bash
# 在CMakeLists.txt中取消注释：
# enable_testing()
# add_subdirectory(tests)

# 然后重新构建并运行测试
cd build
ctest
```

### 代码风格
- 使用Qt编码规范
- 中文注释和变量命名
- 遵循RAII原则进行资源管理

## 许可证

本项目采用MIT许可证。详情请参阅LICENSE文件。

## 贡献

欢迎提交问题报告和功能请求。如需贡献代码，请先fork项目并创建pull request。