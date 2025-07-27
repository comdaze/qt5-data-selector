# Qt5 Data Selector 测试套件

本目录包含Qt5数据筛选器应用程序的完整测试套件，包括单元测试和集成测试。

## 测试结构

```
tests/
├── unit/                           # 单元测试
│   ├── test_timewidget_unit.cpp    # TimeWidget单元测试
│   ├── test_custombutton_unit.cpp  # CustomButton单元测试
│   └── test_devicewidget_unit.cpp  # DeviceWidget单元测试
├── integration/                    # 集成测试
│   └── test_mainwindow_integration.cpp # MainWindow集成测试
├── CMakeLists.txt                  # 测试构建配置
└── README.md                       # 本文档
```

## 测试覆盖范围

### 单元测试

#### 1. TimeWidget单元测试 (`test_timewidget_unit.cpp`)
**需求覆盖**: 1.1, 2.3, 3.1

**测试内容**:
- 初始状态验证
- 时间范围设置和获取
- 时间颗粒度功能
- 快捷时间按钮功能
- 时间验证机制
- 信号发射验证
- 边界条件处理

**主要测试方法**:
- `testInitialState()` - 测试初始状态
- `testTimeRangeChange()` - 测试时间范围变化
- `testTimeValidation()` - 测试时间验证
- `testSignalConnection()` - 测试信号连接
- `testQuickTimeButtons()` - 测试快捷时间按钮

#### 2. CustomButton单元测试 (`test_custombutton_unit.cpp`)
**需求覆盖**: 4.3, 4.4

**测试内容**:
- 选中状态管理
- 悬停效果处理
- 鼠标事件响应
- 状态变化信号
- 样式更新机制
- 状态组合处理

**主要测试方法**:
- `testSetSelected()` - 测试选中状态设置
- `testSelectedSignalEmission()` - 测试选中状态信号
- `testMouseEnterLeave()` - 测试鼠标进入离开
- `testClickEvent()` - 测试点击事件
- `testStateTransitions()` - 测试状态转换

#### 3. DeviceWidget单元测试 (`test_devicewidget_unit.cpp`)
**需求覆盖**: 2.3, 3.1, 3.2, 3.3, 3.4, 3.5

**测试内容**:
- 搜索功能验证
- 设备筛选逻辑
- 设备选择管理
- 数据加载处理
- 用户界面交互
- 错误处理机制

**主要测试方法**:
- `testSearchFiltering()` - 测试搜索过滤
- `testDeviceSelection()` - 测试设备选择
- `testSelectionSignalEmission()` - 测试选择信号
- `testNoMatchResults()` - 测试无匹配结果
- `testFilterHierarchy()` - 测试过滤层级结构

### 集成测试

#### MainWindow集成测试 (`test_mainwindow_integration.cpp`)
**需求覆盖**: 1.1, 2.1, 3.1

**测试内容**:
- 组件间通信验证
- 时间选择与设备筛选联动
- 搜索功能端到端测试
- 状态同步机制
- 复杂用户工作流程
- 错误处理集成
- 性能和稳定性

**主要测试方法**:
- `testTimeRangeDeviceInteraction()` - 测试时间范围和设备交互
- `testSearchEndToEnd()` - 测试搜索端到端流程
- `testComponentStateSynchronization()` - 测试组件状态同步
- `testComplexUserWorkflow()` - 测试复杂用户工作流程
- `testErrorHandlingIntegration()` - 测试错误处理集成

## 运行测试

### 方法1: 使用测试脚本（推荐）

```bash
# 运行所有测试
./run_tests.sh

# 只运行单元测试
./run_tests.sh unit

# 只运行集成测试
./run_tests.sh integration

# 使用CTest运行
./run_tests.sh ctest
```

### 方法2: 使用CMake和CTest

```bash
# 构建项目和测试
mkdir build && cd build
cmake ..
make -j$(nproc)

# 运行所有测试
ctest --output-on-failure

# 运行特定类型的测试
ctest --output-on-failure -R "unit"
ctest --output-on-failure -R "integration"

# 运行特定测试
ctest --output-on-failure -R "test_timewidget_unit"
```

### 方法3: 直接运行测试可执行文件

```bash
cd build

# 运行单元测试
QT_QPA_PLATFORM=offscreen ./tests/unit/test_timewidget_unit
QT_QPA_PLATFORM=offscreen ./tests/unit/test_custombutton_unit
QT_QPA_PLATFORM=offscreen ./tests/unit/test_devicewidget_unit

# 运行集成测试
QT_QPA_PLATFORM=offscreen ./tests/integration/test_mainwindow_integration
```

## 测试环境配置

### 必需依赖
- Qt5 (Core, Widgets, Test模块)
- CMake 3.16+
- C++17兼容编译器

### 环境变量
- `QT_QPA_PLATFORM=offscreen` - 用于无头环境运行GUI测试

### 构建选项
- `CMAKE_BUILD_TYPE=Debug` - 启用调试信息和测试覆盖率
- `CMAKE_BUILD_TYPE=Release` - 发布版本测试

## 测试覆盖率

如果使用GCC编译器并设置了Debug模式，可以生成测试覆盖率报告：

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
make coverage  # 生成覆盖率报告
```

覆盖率报告将生成在 `build/coverage/` 目录中。

## 持续集成

测试套件设计为可在CI/CD环境中运行：

```yaml
# GitHub Actions示例
- name: Run Tests
  run: |
    mkdir build && cd build
    cmake ..
    make -j$(nproc)
    QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

## 测试最佳实践

### 编写新测试时请遵循：

1. **命名约定**: 测试方法使用 `test` 前缀
2. **独立性**: 每个测试应该独立运行
3. **清理**: 使用 `init()` 和 `cleanup()` 方法
4. **断言**: 使用Qt Test框架的断言宏
5. **文档**: 为测试方法添加注释说明

### 测试结构模板：

```cpp
class TestMyComponent : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();    // 测试套件初始化
    void cleanupTestCase(); // 测试套件清理
    void init();           // 每个测试前初始化
    void cleanup();        // 每个测试后清理
    
    void testFeature();    // 具体测试方法
};
```

## 故障排除

### 常见问题

1. **测试无法运行**: 确保设置了 `QT_QPA_PLATFORM=offscreen`
2. **找不到头文件**: 检查include路径配置
3. **链接错误**: 确保Qt5 Test模块已正确链接
4. **信号测试失败**: 使用 `QApplication::processEvents()` 处理事件循环

### 调试测试

```bash
# 启用详细输出
QT_QPA_PLATFORM=offscreen ./test_executable -v2

# 运行特定测试函数
QT_QPA_PLATFORM=offscreen ./test_executable testSpecificFunction
```

## 贡献指南

添加新测试时：

1. 确定测试类型（单元测试或集成测试）
2. 在相应目录创建测试文件
3. 更新 `tests/CMakeLists.txt`
4. 更新本README文档
5. 运行所有测试确保无回归

## 需求映射

| 需求ID | 测试文件 | 测试方法 |
|--------|----------|----------|
| 1.1 | test_timewidget_unit.cpp | testInitialState, testTimeRangeChange |
| 1.2 | test_timewidget_unit.cpp | testTimeValidation |
| 1.3 | test_timewidget_unit.cpp | testQuickTimeButtons |
| 1.4 | test_timewidget_unit.cpp | testPreviousDaySelection |
| 1.5 | test_timewidget_unit.cpp | testEndTimeBeforeStartTime |
| 2.1 | test_mainwindow_integration.cpp | testTimeRangeDeviceInteraction |
| 2.3 | test_devicewidget_unit.cpp | testDeviceSelection |
| 2.4 | test_devicewidget_unit.cpp | testSelectAllFunctionality |
| 2.5 | test_devicewidget_unit.cpp | testSelectedCountUpdate |
| 3.1 | test_devicewidget_unit.cpp | testSearchFiltering |
| 3.2 | test_devicewidget_unit.cpp | testSearchSignalEmission |
| 3.3 | test_devicewidget_unit.cpp | testSearchCaseSensitivity |
| 3.4 | test_devicewidget_unit.cpp | testFilterHierarchy |
| 3.5 | test_devicewidget_unit.cpp | testNoMatchResults |
| 4.3 | test_custombutton_unit.cpp | testSetSelected, testSelectedSignalEmission |
| 4.4 | test_custombutton_unit.cpp | testMouseEnterLeave, testClickEvent |